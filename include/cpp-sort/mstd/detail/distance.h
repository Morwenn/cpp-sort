/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_DETAIL_DISTANCE_H_
#define CPPSORT_MSTD_DETAIL_DISTANCE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include "begin.h"
#include "end.h"
#include "range.h"
#include "range_traits.h"
#include "size.h"

namespace cppsort::mstd
{
    namespace detail_distance
    {
        struct fn
        {
            template<typename Iterator, sentinel_for<Iterator> Sentinel>
                requires (not sized_sentinel_for<Sentinel, Iterator>)
            [[nodiscard]]
            constexpr auto operator()(Iterator first, Sentinel last) const
                -> iter_difference_t<Iterator>
            {
                iter_difference_t<Iterator> n = 0;
                while (first != last) {
                    ++first;
                    ++n;
                }
                return n;
            }

            template<
                typename Iterator,
                sized_sentinel_for<std::decay_t<Iterator>> Sentinel
            >
            [[nodiscard]]
            constexpr auto operator()(Iterator&& first, Sentinel last) const
                -> iter_difference_t<std::decay_t<Iterator>>
            {
                if constexpr (sized_sentinel_for<Sentinel, std::remove_cvref_t<Iterator>>) {
                    return last - first;
                } else {
                    return last - static_cast<const std::decay_t<Iterator>&>(first);
                }
            }

            template<range Range>
            [[nodiscard]]
            constexpr auto operator()(Range&& range) const
                -> range_difference_t<Range>
            {
                if constexpr (sized_range<Range>) {
                    return static_cast<range_difference_t<Range>>(mstd::size(range));
                } else {
                    return operator()(mstd::begin(range), mstd::end(range));
                }
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto distance = detail_distance::fn{};
    }
}

#endif // CPPSORT_MSTD_DETAIL_DISTANCE_H_
