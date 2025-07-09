/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_EXC_H_
#define CPPSORT_PROBES_EXC_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
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
        constexpr auto compare_equivalent(ForwardIterator it1, ForwardIterator it2,
                                          Compare compare, Projection projection)
            -> bool
        {
            return not compare(projection(*it1), projection(*it2))
                && not compare(projection(*it2), projection(*it1));
        }

        template<typename ForwardIterator, typename Compare, typename Projection>
        auto exc_probe_algo(ForwardIterator first, ForwardIterator last,
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
                iterators.begin(), iterators.end(),
                compare, utility::indirect{} | projection
            );

            ////////////////////////////////////////////////////////////
            // Count the number of cycles

            // Element where the current cycle starts
            auto start = first;

            difference_type cycles = 0;
            do {
                // Find the element to put in current's place
                auto current = start;
                auto next_pos = std::distance(first, current);
                // We replace all "sorted" iterators with last to make it
                // possible to find unsorted iterators between cycles
                auto next = std::exchange(iterators[next_pos], last);

                // Process the current cycle
                if (next == current) {
                    // An element already in its final position counts as one cycle
                    ++cycles;
                } else {
                    bool all_equivalent = true;

                    // If an element is in the place of another element that compares
                    // equivalent, it means that this element was actually already in
                    // a suitable place, so we count one more cycle as if it was an
                    // already suitably placed element, this handles collections with
                    // several elements which compare equivalent
                    if (compare_equivalent(current, next, comp, proj)) {
                        ++cycles;
                    } else {
                        all_equivalent = false;
                    }

                    while (next != start) {
                        // Locate the next element of the cycle
                        current = next;
                        auto next_pos = std::distance(first, next);
                        next = std::exchange(iterators[next_pos], last);

                        if (compare_equivalent(current, next, comp, proj)) {
                            ++cycles;
                        } else {
                            all_equivalent = false;
                        }
                    }

                    // If all elements of a cycle compare equivalent, we already added one
                    // cycle per element, so we only add one there if at least two elements
                    // compared different
                    if (not all_equivalent) {
                        ++cycles;
                    }
                }

                // Find the next cycle
                auto&& iterators_it = iterators.begin() + std::distance(first, start);
                do {
                    ++start;
                    ++iterators_it;
                } while (start != last && *iterators_it == last);
            } while (start != last);

            return size - cycles;
        }

        struct exc_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return exc_probe_algo(std::begin(iterable), std::end(iterable),
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
                return exc_probe_algo(first, last, std::distance(first, last),
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

    namespace
    {
        constexpr auto&& exc = utility::static_const<
            sorter_facade<detail::exc_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_EXC_H_
