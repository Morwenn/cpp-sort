/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_MONO_H_
#define CPPSORT_PROBES_MONO_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>

namespace cppsort::probe
{
    namespace detail
    {
        struct mono_impl
        {
            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            constexpr auto operator()(Iterator first, Sentinel last,
                                      Compare compare={}, Projection projection={}) const
                -> mstd::iter_difference_t<Iterator>
            {
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last || std::next(first) == last) {
                    return 0;
                }

                auto current = first;
                auto next = std::next(first);

                // The result of mono is the number of ascending runs founds +
                // the number of descending runs founds minus one (that minus
                // one is needed to make mono a proper measure of presortedness,
                // returning 0 when the collection is sorted), so the whole
                // algorithm is designed in a way that it will be "late by one"
                // on the count of runs
                mstd::iter_difference_t<Iterator> count = 0;

                while (next != last) {

                    if (comp(proj(*current), proj(*next))) {
                        // Look for an ascending run
                        do {
                            current = next;
                            ++next;
                            if (next == last) {
                                return count;
                            }
                        } while(not comp(proj(*next), proj(*current)));
                        ++count;

                    } else if (comp(proj(*next), proj(*current))) {
                        // Look for a descending run
                        do {
                            current = next;
                            ++next;
                            if (next == last) {
                                return count;
                            }
                        } while(not comp(proj(*current), proj(*next)));
                        ++count;
                    }

                    //
                    // When we have reached the end of an ascending or descending run,
                    // we need to increase the current and next iterators in order to
                    // reach the next pair to compute
                    //
                    // We also try to avoid being biased towards either ascending or
                    // descending runs: when two values compare equivalent, increment the
                    // iterators and wait until two elements compare unequal or until the
                    // end of the collection has been reached
                    //
                    current = next;
                    ++next;
                }
                return count;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : (n + 1) / 2 - 1;
            }
        };
    }

    inline constexpr sorter_facade<detail::mono_impl> mono{};
}

#endif // CPPSORT_PROBES_MONO_H_
