/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_MAX_H_
#define CPPSORT_PROBES_MAX_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/equal_range.h"
#include "../detail/functional.h"
#include "../detail/immovable_vector.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto max_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = ::cppsort::detail::difference_type_t<ForwardIterator>;
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
            // Maximum distance an element has to travel in order to
            // reach its sorted position

            difference_type max_dist = 0;
            difference_type it_pos = 0;
            for (auto it = first ; it != last ; ++it) {
                // Find the range where *first belongs once sorted
                auto rng = cppsort::detail::equal_range(
                    iterators.begin(), iterators.end(), proj(*it),
                    compare, cppsort::detail::indirect(projection)
                );
                auto pos_min = std::distance(iterators.begin(), rng.first);
                auto pos_max = std::distance(iterators.begin(), rng.second);

                // If *first isn't into one of its sorted positions, computed the closest
                if (it_pos < pos_min) {
                    max_dist = (std::max)(pos_min - it_pos, max_dist);
                } else if (it_pos >= pos_max) {
                    max_dist = (std::max)(it_pos - pos_max + 1, max_dist);
                }

                ++it_pos;
            }
            return max_dist;
        }

        struct max_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, Range, Compare>
                >
            >
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return max_probe_algo(std::begin(range), std::end(range),
                                      mstd::distance(range),
                                      std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Iterator last,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return max_probe_algo(first, last,
                                      std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n - 1;
            }
        };
    }

    inline constexpr sorter_facade<detail::max_impl> max{};
}

#endif // CPPSORT_PROBES_MAX_H_
