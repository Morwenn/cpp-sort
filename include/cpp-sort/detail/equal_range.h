/*
 * Copyright (c) 2019-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_EQUAL_RANGE_H_
#define CPPSORT_DETAIL_EQUAL_RANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "lower_bound.h"
#include "upper_bound.h"

namespace cppsort::detail
{
    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto equal_range(ForwardIterator first, ForwardIterator last, const T& value,
                     Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        using difference_type = mstd::iter_difference_t<ForwardIterator>;
        difference_type len = mstd::distance(first, last);
        while (len != 0) {
            difference_type l2 = half(len);
            auto m = first;
            mstd::advance(m, l2);
            if (comp(proj(*m), value)) {
                first = ++m;
                len -= l2 + 1;
            } else if (comp(value, proj(*m))) {
                last = m;
                len = l2;
            } else {
                auto mp1 = m;
                return std::make_pair(
                    lower_bound(first, m, value, compare, projection),
                    upper_bound(++mp1, last, value, compare, projection)
                );
            }
        }
        return std::make_pair(first, first);
    }
}

#endif // CPPSORT_DETAIL_EQUAL_RANGE_H_
