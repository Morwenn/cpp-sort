/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_EXC_H_
#define CPPSORT_PROBES_EXC_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/immovable_vector.h"
#include "../detail/pdqsort.h"

namespace cppsort::probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Sentinel, typename Compare, typename Projection>
        auto exc_probe_algo(ForwardIterator first, Sentinel last,
                            mstd::iter_difference_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> mstd::iter_difference_t<ForwardIterator>
        {
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
                iterators.begin(), iterators.end(),
                compare, utility::indirect{} | projection
            );

            ////////////////////////////////////////////////////////////
            // Count the number of cycles

            std::vector<bool> sorted(size, false);

            // Element where the current cycle starts
            auto start = first;

            mstd::iter_difference_t<ForwardIterator> cycles = 0;
            while (start != last) {
                // Find the element to put in current's place
                auto current = start;
                auto next_pos = mstd::distance(first, current);
                auto next = iterators[next_pos];
                sorted[next_pos] = true;

                // Process the current cycle
                if (next != current) {
                    while (next != start) {
                        // If an element is in the place of another element that compares
                        // equivalent, it means that this element was actually already in
                        // a suitable place, so we count one more cycle as if it was an
                        // already suitably placed element, this handles collections with
                        // several elements which compare equivalent
                        if (not comp(proj(*next), proj(*current)) &&
                            not comp(proj(*current), proj(*next))) {
                            ++cycles;
                        }
                        // Locate the next element of the cycle
                        current = next;
                        auto next_pos = mstd::distance(first, next);
                        next = iterators[next_pos];
                        sorted[next_pos] = true;
                    }
                }

                ++cycles;

                // Find the next cycle
                auto&& sorted_it = sorted.begin() + mstd::distance(first, start);
                do {
                    ++start;
                    ++sorted_it;
                } while (start != last && *sorted_it);
            }
            return size - cycles;
        }

        struct exc_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> mstd::range_difference_t<Range>
            {
                return exc_probe_algo(mstd::begin(range), mstd::end(range),
                                      mstd::distance(range),
                                      std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> mstd::iter_difference_t<Iterator>
            {
                return exc_probe_algo(first, last,
                                      mstd::distance(first, last),
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

    inline constexpr sorter_facade_base<detail::exc_impl> exc{};
}

#endif // CPPSORT_PROBES_EXC_H_
