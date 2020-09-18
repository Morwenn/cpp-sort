/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto equal_range(ForwardIterator first, ForwardIterator last, const T& value,
                     Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        using difference_type = difference_type_t<ForwardIterator>;
        difference_type len = std::distance(first, last);
        while (len != 0) {
            difference_type l2 = half(len);
            ForwardIterator m = first;
            std::advance(m, l2);
            if (comp(proj(*m), value)) {
                first = ++m;
                len -= l2 + 1;
            } else if (comp(value, proj(*m))) {
                last = m;
                len = l2;
            } else {
                ForwardIterator mp1 = m;
                return std::make_pair(
                    lower_bound(first, m, value, compare, projection),
                    upper_bound(++mp1, last, value, compare, projection)
                );
            }
        }
        return std::make_pair(first, first);
    }
}}

#endif // CPPSORT_DETAIL_EQUAL_RANGE_H_
