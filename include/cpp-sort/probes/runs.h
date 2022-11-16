/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_RUNS_H_
#define CPPSORT_PROBES_RUNS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>

namespace cppsort::probe
{
    namespace detail
    {
        struct runs_impl
        {
            template<
                mstd::forward_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
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

                mstd::iter_difference_t<Iterator> count = 0;
                while (true) {
                    while (next != last && not comp(proj(*next), proj(*current))) {
                        ++current;
                        ++next;
                    }

                    if (next == last) break;
                    ++count;
                    ++current;
                    ++next;
                }
                return count;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n - 1;
            }
        };
    }

    inline constexpr sorter_facade<detail::runs_impl> runs{};
}

#endif // CPPSORT_PROBES_RUNS_H_
