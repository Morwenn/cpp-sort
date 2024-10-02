/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_REM_H_
#define CPPSORT_PROBES_REM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/longest_non_descending_subsequence.h"

namespace cppsort::probe
{
    namespace detail
    {
        struct rem_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires mstd::sized_range<Range> &&
                        is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> mstd::range_difference_t<Range>
            {
                // While most algorithms use mstd::distance() for everything, we only
                // want to handle data structures whose size can be computed in O(1),
                // otherwise it is cheaper to let longest_non_descending_subsequence
                // compute the size itself.
                auto res = cppsort::detail::longest_non_descending_subsequence<false>(
                    mstd::begin(range), mstd::end(range),
                    mstd::size(range),
                    std::move(compare), std::move(projection)
                );
                auto lnds_size = res.second - res.first;
                return lnds_size >= 0 ? lnds_size : 0;
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
                // We give 0 as a "dummy" value since it will be recomputed, but it
                // is also used by the non-random-access iterators version as the
                // initial value used for the size count
                auto res = cppsort::detail::longest_non_descending_subsequence<true>(
                    first, last, 0, std::move(compare), std::move(projection)
                );
                auto lnds_size = res.second - res.first;
                return lnds_size >= 0 ? lnds_size : 0;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n - 1;
            }
        };
    }

    inline constexpr sorter_facade_base<detail::rem_impl> rem{};
}

#endif // CPPSORT_PROBES_REM_H_
