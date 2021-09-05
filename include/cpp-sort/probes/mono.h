/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_MONO_H_
#define CPPSORT_PROBES_MONO_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct mono_impl
        {
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
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
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
                difference_type count = 0;

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

    namespace
    {
        constexpr auto&& mono = utility::static_const<
            sorter_facade<detail::mono_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_MONO_H_
