/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SLABSORT_H_
#define CPPSORT_DETAIL_SLABSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <vector>
#include <cpp-sort/adapters/stable_adapter.h>
#include "bitops.h"
#include "fixed_size_list.h"
#include "functional.h"
#include "immovable_vector.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "melsort.h"
#include "stable_partition.h"
#include "nth_element.h"

namespace cppsort
{
namespace detail
{
    // Basically list_node, except that the internal union contains
    // either a value or an iterator pointing to that value. This
    // allows to perform operations indirectly in a first time, then
    // to move the values inside the list at some point.

    template<typename Iterator>
    struct slabsort_list_node:
        list_node_base
    {
        using value_type = rvalue_type_t<Iterator>;

        using list_node_base::list_node_base;

        explicit slabsort_list_node(list_node_base* next) noexcept:
            list_node_base(next)
        {}

        slabsort_list_node(list_node_base* prev, list_node_base* next) noexcept:
            list_node_base(prev, next)
        {}

        ~slabsort_list_node() {}

        union {
            value_type value;
            Iterator it;
        };
    };

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto slabsort_get_median(RandomAccessIterator first, RandomAccessIterator last,
                             immovable_vector<RandomAccessIterator>& iterators_buffer,
                             Compare compare, Projection projection)
        -> RandomAccessIterator
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        auto size = last - first;

        ////////////////////////////////////////////////////////////
        // Bind index to iterator

        // Associate iterators to their position
        iterators_buffer.clear();
        for (difference_type count = 0; count != size; ++count) {
            iterators_buffer.emplace_back(first);
            ++first;
        }

        ////////////////////////////////////////////////////////////
        // Run nth_element stably and indirectly

        return *nth_element(
            iterators_buffer.begin(), iterators_buffer.end(), size / 2, size,
            std::move(compare), indirect(std::move(projection))
        );
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto slabsort_partition(RandomAccessIterator first, RandomAccessIterator last,
                            immovable_vector<RandomAccessIterator>& iterators_buffer,
                            Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Note: the partition algorithm is supposed to partition stably in
        //       order to preserve the presortedness that is suitable for
        //       melsort, however we don't fully partition stably here: the
        //       last element and pivot will not be ordered stably, but that
        //       difference should not have any noticeable impact on the
        //       adaptivity to presortedness

        auto pivot = slabsort_get_median(first, last, iterators_buffer, compare, projection);
        auto last_1 = std::prev(last);

        // Put the pivot at position std::prev(last) and partition
        iter_swap(pivot, last_1);
        auto&& pivot1 = proj(*last_1);
        auto middle1 = detail::stable_partition(
            first, last_1,
            [&](auto&& elem) { return comp(proj(elem), pivot1); }
        );

        // Put the pivot back in its final position
        iter_swap(middle1, last_1);
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto try_melsort(RandomAccessIterator first, RandomAccessIterator last,
                     difference_type_t<RandomAccessIterator> p,
                     fixed_size_list_node_pool<slabsort_list_node<RandomAccessIterator>>& node_pool,
                     Compare compare, Projection projection)
        -> bool
    {
        using rvalue_type = rvalue_type_t<RandomAccessIterator>;
        using node_type = slabsort_list_node<RandomAccessIterator>;
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (first == last || std::next(first) == last) {
            return true;
        }

        // Encroaching lists
        std::vector<fixed_size_list<node_type>> lists;
        lists.emplace_back(node_pool, destroy_node_contents<RandomAccessIterator, node_type, &node_type::it>);
        lists.back().push_back([&first](node_type* node) {
            ::new (&node->it) RandomAccessIterator(first);
        });

        ////////////////////////////////////////////////////////////
        // Create encroaching lists

        for (auto it = std::next(first) ; it != last ; ++it) {
            auto&& value = proj(*it);

            auto& last_list = lists.back();
            if (not comp(value, proj(*std::prev(last_list.end()).base()->it))) {
                // Element belongs to the tails (bigger elements)
                auto insertion_point = detail::lower_bound(
                    lists.begin(), std::prev(lists.end()), value, invert(compare),
                    [&proj](auto& list) -> decltype(auto) {
                        return proj(*std::prev(list.end()).base()->it);
                    }
                );
                insertion_point->push_back([&it](node_type* node) {
                    ::new (&node->it) RandomAccessIterator(it);
                });
            } else if (not comp(proj(*last_list.begin().base()->it), value)) {
                // Element belongs to the heads (smaller elements)
                auto insertion_point = detail::lower_bound(
                    lists.begin(), std::prev(lists.end()), value, compare,
                    [&proj](auto& list) -> decltype(auto) {
                        return proj(*list.begin().base()->it);
                    }
                );
                insertion_point->push_front([&it](node_type* node) {
                    ::new (&node->it) RandomAccessIterator(it);
                });
            } else {
                // Element does not belong to the existing encroaching lists,
                // create a new list for it
                lists.emplace_back(node_pool, destroy_node_contents<RandomAccessIterator, node_type, &node_type::it>);
                lists.back().push_back([&it](node_type* node) {
                    ::new (&node->it) RandomAccessIterator(it);
                });
            }

            // Too many encroaching lists have been created (Enc > p),
            // give up, the elements of the collection remain in their
            // order of creation
            using difference_type = difference_type_t<RandomAccessIterator>;
            if (difference_type(lists.size()) >= p) {
                return false;
            }
        }

        ////////////////////////////////////////////////////////////
        // Reify encroaching lists

        for (auto& list : lists) {
            // TODO: handle destruction when an exception is thrown during the loop
            for (auto it = list.begin(), end = list.end() ; it != end ; ++it) {
                auto node = it.base();
                auto value_it = node->it;
                detail::destroy_at(&node->it);
                ::new(&node->value) rvalue_type(iter_move(value_it));
            }
            list.set_node_destructor(destroy_node_contents<rvalue_type, node_type, &node_type::value>);
        }

        ////////////////////////////////////////////////////////////
        // Merge encroaching lists

        // We only ever reach this point in the algorithm when only
        // a few encroaching lists have been created, therefore the
        // edges extraction optimization can never be worth it
        merge_encroaching_lists(lists, first, false, std::move(compare), std::move(projection));
        return true;
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto slabsort_impl(RandomAccessIterator first, RandomAccessIterator last,
                       difference_type_t<RandomAccessIterator> size,
                       difference_type_t<RandomAccessIterator> original_p,
                       difference_type_t<RandomAccessIterator> current_p,
                       immovable_vector<RandomAccessIterator>& iterators_buffer,
                       fixed_size_list_node_pool<slabsort_list_node<RandomAccessIterator>>& node_pool,
                       Compare compare, Projection projection)
        -> void
    {
        if (size < 2) {
            return;
        }

        slabsort_partition(first, last, iterators_buffer, compare, projection);
        auto left_size = (last - first) / 2;
        auto right_size = size - left_size;
        auto middle = first + left_size;
        if (current_p > 2) {
            // Partition further until the partitions are small enough
            slabsort_impl(first, middle, left_size, original_p, current_p / 2,
                          iterators_buffer, node_pool, compare, projection);
            slabsort_impl(middle, last, right_size, original_p, current_p / 2,
                          iterators_buffer, node_pool, compare, projection);
        } else {
            // The partitions are small enough, try to use melsort on them,
            // if too many encroaching lists are created, cancel and recurse
            bool done = try_melsort(first, middle, original_p, node_pool, compare, projection);
            if (not done) {
                slabsort_impl(first, middle, left_size,
                              original_p * original_p, original_p * original_p,
                              iterators_buffer, node_pool, compare, projection);
            }
            done = try_melsort(middle, last, original_p, node_pool, compare, projection);
            if (not done) {
                slabsort_impl(middle, last, right_size,
                              original_p * original_p, original_p * original_p,
                              iterators_buffer, node_pool, compare, projection);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto slabsort(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection)
        -> void
    {
        auto size = last - first;
        if (size < 2) {
            return;
        }

        // Node pool used by all try_melsort invocations
        using node_type = slabsort_list_node<RandomAccessIterator>;
        fixed_size_list_node_pool<node_type> node_pool(size);

        // Take advantage of existing presortedness once before the partitioning
        // partly gets rid of it, this makes a difference for collections with
        // a few bigs runs
        bool done = try_melsort(first, last, 2 * detail::log2(size),
                                node_pool, compare, projection);
        if (done) {
            return;
        }

        // Allocate a buffer that will be used for median finding
        immovable_vector<RandomAccessIterator> iterators_buffer(size);

        difference_type_t<RandomAccessIterator> original_p = 2;
        return slabsort_impl(
            first, last, size, original_p, original_p,
            iterators_buffer, node_pool,
            std::move(compare), std::move(projection)
        );
    }
}}

#endif // CPPSORT_DETAIL_SLABSORT_H_
