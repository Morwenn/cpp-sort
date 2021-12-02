/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_OSC_H_
#define CPPSORT_PROBES_OSC_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <new>
#include <numeric>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/equal_range.h"
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto inplace_osc_algo(ForwardIterator first, ForwardIterator last,
                              cppsort::detail::difference_type_t<ForwardIterator> size,
                              Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            // Deprecated in-place O(n^2) algorithm

            using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (size < 2) {
                return 0;
            }

            difference_type count = 0;
            for (auto it = first; it != last; ++it) {
                auto&& value = proj(*it);

                auto current = first;
                auto next = std::next(first);

                while (next != last) {
                    if (comp((std::min)(proj(*current), proj(*next), comp), value) &&
                        comp(value, (std::max)(proj(*current), proj(*next), comp))) {
                        ++count;
                    }
                    ++current;
                    ++next;
                }
            }
            return count;
        }

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto allocating_osc_algo(ForwardIterator first, ForwardIterator last,
                                 cppsort::detail::difference_type_t<ForwardIterator> size,
                                 Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (size < 2) {
                return 0;
            }

            ////////////////////////////////////////////////////////////
            // Indirectly sort the iterators

            // Copy the iterators in a vector
            cppsort::detail::immovable_vector<ForwardIterator> iterators(size);
            for (auto it = first; it != last; ++it) {
                iterators.emplace_back(it);
            }

            // Sort the iterators on pointed values
            cppsort::detail::pdqsort(
                iterators.begin(), iterators.end(), compare,
                cppsort::detail::indirect(projection)
            );

            ////////////////////////////////////////////////////////////
            // Compute the oscillation

            // This algorithm was proposed by Control and works as follow:
            // initialize a vector of N 0, then for each pair of elements
            // in the collection (ex: (a,b), (b,c), (c,d), etc.), find the
            // position of the elements of the pair in the sorted collection
            // (pos_min and pos_max), increment cross[pos_min + 1] and
            // decrement cross[pos_max - 1]. Then compute the prefix sum of
            // cross, the oscillation is the sum of that prefix sum.

            // Note: in the following algorithm, all upper_bound calls are
            //       performed on the full iterator sequence without taking
            //       advantage of the information we already have to reduce
            //       the search space. This is because benchmarks reported
            //       that reducing the search space made the algorithm up to
            //       twice as slow. Comments in the code contain the lines
            //       required to reduce the search space again.

            std::vector<difference_type> cross(size, 0);

            auto prev_bounds = cppsort::detail::equal_range(
                iterators.begin(), iterators.end(), proj(*first),
                compare, cppsort::detail::indirect(projection)
            );

            for (auto prev = first, current = std::next(prev); current != last; ++prev, (void)++current) {
                difference_type min_idx, max_idx;
                if (comp(proj(*prev), proj(*current))) {
                    auto current_bounds = cppsort::detail::equal_range(
                        //prev_bounds.second, std::prev(iterators.end()), proj(*current),
                        iterators.begin(), iterators.end(), proj(*current),
                        compare, cppsort::detail::indirect(projection)
                    );
                    min_idx = prev_bounds.second - iterators.begin();
                    max_idx = current_bounds.first - iterators.begin();
                    prev_bounds = current_bounds;
                } else if (comp(proj(*current), proj(*prev))) {
                    auto current_bounds = cppsort::detail::equal_range(
                        //iterators.begin(), prev_bounds.first, proj(*current),
                        iterators.begin(), iterators.end(), proj(*current),
                        compare, cppsort::detail::indirect(projection)
                    );
                    min_idx = current_bounds.second - iterators.begin();
                    max_idx = prev_bounds.first - iterators.begin();
                    prev_bounds = current_bounds;
                } else {
                    // *prev == *current, bounds don't change
                    continue;
                }

                cross[min_idx] += 1;
                cross[max_idx] -= 1;
            }

            std::partial_sum(cross.begin(), cross.end(), cross.begin());
            return std::accumulate(cross.begin(), cross.end(), difference_type(0));
        }

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto osc_algo(ForwardIterator first, ForwardIterator last,
                      cppsort::detail::difference_type_t<ForwardIterator> size,
                      Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            try {
                return allocating_osc_algo(first, last, size, compare, projection);
            } catch (std::bad_alloc&) {
                return inplace_osc_algo(
                    first, last, size,
                    std::move(compare), std::move(projection)
                );
            }
        }

        struct osc_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable, Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return osc_algo(std::begin(iterable), std::end(iterable),
                                utility::size(iterable),
                                std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return osc_algo(first, last, std::distance(first, last),
                                std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : (n * (n - 2) - 1) / 2;
            }
        };
    }

    namespace
    {
        constexpr auto&& osc = utility::static_const<
            sorter_facade<detail::osc_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_OSC_H_
