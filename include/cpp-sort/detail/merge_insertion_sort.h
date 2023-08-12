/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
#define CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "attributes.h"
#include "fixed_size_list.h"
#include "immovable_vector.h"
#include "iterator_traits.h"
#include "move.h"
#include "scope_exit.h"
#include "swap_if.h"
#include "swap_ranges.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Iterator used to sort groups

    template<typename Iterator>
    class group_iterator
    {
        public:

            ////////////////////////////////////////////////////////////
            // Public types

            using iterator_category = iterator_category_t<Iterator>;
            using iterator_type     = Iterator;
            using value_type        = value_type_t<Iterator>;
            using difference_type   = difference_type_t<Iterator>;
            using pointer           = pointer_t<Iterator>;
            using reference         = reference_t<Iterator>;

            ////////////////////////////////////////////////////////////
            // Constructors

            group_iterator() = default;

            group_iterator(Iterator it, difference_type size):
                _it(std::move(it)),
                _size(size)
            {}

            ////////////////////////////////////////////////////////////
            // Members access

            CPPSORT_ATTRIBUTE_NODISCARD
            auto base() const
                -> iterator_type
            {
                return _it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            auto size() const
                -> difference_type
            {
                return _size;
            }

            ////////////////////////////////////////////////////////////
            // Element access

            CPPSORT_ATTRIBUTE_NODISCARD
            auto operator*() const
                -> reference
            {
                return *std::next(_it, _size - 1);
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            auto operator->() const
                -> pointer
            {
                return &(operator*());
            }

            ////////////////////////////////////////////////////////////
            // Increment/decrement operators

            auto operator++()
                -> group_iterator&
            {
                std::advance(_it, _size);
                return *this;
            }

            auto operator--()
                -> group_iterator&
            {
                std::advance(_it, -_size);
                return *this;
            }

            auto operator+=(difference_type increment)
                -> group_iterator&
            {
                _it += _size * increment;
                return *this;
            }

            auto operator-=(difference_type increment)
                -> group_iterator&
            {
                _it -= _size * increment;
                return *this;
            }

            ////////////////////////////////////////////////////////////
            // Comparison operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator==(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() == rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator!=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() != rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Relational operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() < rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator<=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() <= rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() > rhs.base();
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator>=(const group_iterator& lhs, const group_iterator& rhs)
                -> bool
            {
                return lhs.base() >= rhs.base();
            }

            ////////////////////////////////////////////////////////////
            // Arithmetic operators

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(group_iterator it, difference_type size)
                -> group_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator+(difference_type size, group_iterator it)
                -> group_iterator
            {
                it += size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(group_iterator it, difference_type size)
                -> group_iterator
            {
                it -= size;
                return it;
            }

            CPPSORT_ATTRIBUTE_NODISCARD
            friend auto operator-(const group_iterator& lhs, const group_iterator& rhs)
                -> difference_type
            {
                return (lhs.base() - rhs.base()) / lhs.size();
            }

            ////////////////////////////////////////////////////////////
            // iter_swap

            friend auto iter_swap(group_iterator lhs, group_iterator rhs)
                -> void
            {
                detail::swap_ranges_inner(lhs.base(), std::next(lhs.base(), lhs.size()), rhs.base());
            }

        private:

            Iterator _it;
            difference_type _size;
    };

    ////////////////////////////////////////////////////////////
    // Construction function

    template<typename Iterator>
    CPPSORT_ATTRIBUTE_NODISCARD
    auto make_group_iterator(Iterator it, difference_type_t<Iterator> size)
        -> group_iterator<Iterator>
    {
        return { it, size };
    }

    template<typename Iterator>
    CPPSORT_ATTRIBUTE_NODISCARD
    auto make_group_iterator(group_iterator<Iterator> it, difference_type_t<Iterator> size)
        -> group_iterator<Iterator>
    {
        size *= it.size();
        return { it.base(), size };
    }

    ////////////////////////////////////////////////////////////
    // Merge-insertion sort

    template<typename RandomAccessIterator, typename NodeType, typename Compare, typename Projection>
    auto merge_insertion_sort_impl(group_iterator<RandomAccessIterator> first,
                                   group_iterator<RandomAccessIterator> last,
                                   fixed_size_list_node_pool<NodeType>& node_pool,
                                   Compare compare, Projection projection)
        -> void
    {
        auto size = last - first;
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        // Whether there is a stray element not in a pair
        // at the end of the chain
        bool has_stray = (size % 2 != 0);

        ////////////////////////////////////////////////////////////
        // Group elements by pairs

        auto end = has_stray ? std::prev(last) : last;
        for (auto it = first ; it != end ; it += 2) {
            iter_swap_if(it, std::next(it), compare, projection);
        }

        ////////////////////////////////////////////////////////////
        // Recursively sort the pairs by max

        merge_insertion_sort_impl(
            make_group_iterator(first, 2),
            make_group_iterator(end, 2),
            node_pool, compare, projection
        );

        ////////////////////////////////////////////////////////////
        // Separate main chain and pend elements

        using list_t = fixed_size_list<list_node<group_iterator<RandomAccessIterator>>>;

        // Reusing the node pool allows to halve the number of nodes
        // allocated by the algorithm, but we need to reset the links
        // between the nodes so that the future allocations remain
        // somewhat cache-friendly - it is theoretically more work,
        // but benchmarks proved that it made a huge difference
        auto node_pool_reset = make_scope_exit([&node_pool, size, group_size=first.size()] {
            if (group_size > 1) {
                node_pool.reset_nodes(size);
            }
        });
        node_pool_reset.deactivate();

        // The first pend element is always part of the main chain,
        // so we can safely initialize the list with the first two
        // elements of the sequence
        list_t chain(node_pool);
        chain.push_back(first);
        chain.push_back(std::next(first));

        // Upper bounds for the insertion of pend elements
        immovable_vector<typename list_t::iterator> pend((size + 1) / 2 - 1);

        for (auto it = first + 2 ; it != end ; it += 2) {
            auto tmp = chain.insert(chain.end(), std::next(it));
            pend.emplace_back(tmp);
        }

        // Add the last element to pend if it exists; when it
        // exists, it always has to be inserted in the full chain,
        // so giving it chain.end() as end insertion point is ok
        if (has_stray) {
            pend.emplace_back(chain.end());
        }

        ////////////////////////////////////////////////////////////
        // Binary insertion into the main chain

        auto current_it = first;
        auto current_pend = pend.begin();

        // At each cycle, we need to find the optimal pend element where to
        // start the insertion cycle again in a way that minimizes the number
        // of comparisons performed. The indices of the optimal sequence of
        // pend elements happens to follow a sequence known as Jacobsthal
        // numbers: https://oeis.org/A001045
        //
        // Due to the element-per-element nature of the algorithm we are using,
        // we don't need to know the position of a pend element, but its
        // relative position to the previous one, in other words we don't need
        // J(n+1), but J(n+1) - J(n). Thanks to the following equivalence it
        // can be computed fairly cheaply:
        // J(n+1) = 2^n - J(n)
        // J(n+2) - J(n+1) = 2^(n+1) - J(n+1) - J(n+1)
        //                 = 2^(n+1) - 2J(n+1)
        //                 = 2(2^n - J(n+1))
        //                 = 2(2^n - (2^n - J(n)))
        //                 = 2(2^n - 2^n + Jn)
        //                 = 2J(n)
        // By keeping track of powers of two and J(n), J(n+1) - J(n) can thus
        // be computed with two shifts and a subtraction.
        for (typename list_t::difference_type pow2 = 1, jn = 0, dist = 2;
             dist <= pend.end() - current_pend;
             pow2 *= 2, jn = pow2 - jn, dist = 2 * jn) {

            auto it = current_it + dist * 2;
            auto pe = current_pend + dist;

            while (true) {
                --pe;

                auto insertion_point = detail::upper_bound(
                    chain.begin(), *pe, proj(*it),
                    compare, utility::indirect{} | projection
                );
                chain.insert(insertion_point, it);

                if (pe == current_pend) break;
                it -= 2;
            }

            current_it += dist * 2;
            current_pend += dist;
        }

        // If there are pend elements left, insert them into the main
        // chain, the order of insertion does not matter so forward
        // traversal is ok
        while (current_pend != pend.end()) {
            current_it += 2;
            auto insertion_point = detail::upper_bound(
                chain.begin(), *current_pend, proj(*current_it),
                compare, utility::indirect{} | projection
            );
            chain.insert(insertion_point, current_it);
            ++current_pend;
        }

        ////////////////////////////////////////////////////////////
        // Move values in order to a cache then back to origin

        // Number of sub-iterators
        auto full_size = size * first.size();

        immovable_vector<rvalue_type_t<RandomAccessIterator>> cache(full_size);
        for (auto&& it: chain) {
            auto begin = it.base();
            auto end = begin + it.size();
            cache.insert_back(begin, end);
        }
        detail::move(cache.begin(), cache.end(), first.base());

        // Everything else worked, it's now safe to reset the node pool
        node_pool_reset.activate();
    }

    template<
        typename RandomAccessIterator,
        typename Compare,
        typename Projection
    >
    auto merge_insertion_sort(RandomAccessIterator first, RandomAccessIterator last,
                              Compare compare, Projection projection)
        -> void
    {
        auto size = last - first;
        if (size < 2) {
            return;
        }

        // Make a node pool big enough to hold all the values
        using node_type = list_node<group_iterator<RandomAccessIterator>>;
        fixed_size_list_node_pool<node_type> node_pool(size);

        merge_insertion_sort_impl(
            make_group_iterator(std::move(first), 1),
            make_group_iterator(std::move(last), 1),
            node_pool,
            std::move(compare), std::move(projection)
        );
    }
}}

#endif // CPPSORT_DETAIL_MERGE_INSERTION_SORT_H_
