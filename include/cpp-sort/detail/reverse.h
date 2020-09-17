/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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

//===-------------------------- algorithm ---------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_REVERSE_H_
#define CPPSORT_DETAIL_REVERSE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator>
    auto reverse_impl(BidirectionalIterator first, BidirectionalIterator last,
                      std::bidirectional_iterator_tag)
        -> void
    {
        using utility::iter_swap;

        while (first != last)
        {
            if (first == --last) break;
            iter_swap(first, last);
            ++first;
        }
    }

    template<typename RandomAccessIterator>
    auto reverse_impl(RandomAccessIterator first, RandomAccessIterator last,
                      std::random_access_iterator_tag)
        -> void
    {
        if (first != last)
        {
            for (; first < --last ; ++first)
            {
                using utility::iter_swap;
                iter_swap(first, last);
            }
        }
    }

    template<typename BidirectionalIterator>
    auto reverse(BidirectionalIterator first, BidirectionalIterator last)
        -> void
    {
        reverse_impl(std::move(first), std::move(last),
                     iterator_category_t<BidirectionalIterator>{});
    }
}}

#endif // CPPSORT_DETAIL_REVERSE_H_
