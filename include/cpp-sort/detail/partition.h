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

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_PARTITION_H_
#define CPPSORT_DETAIL_PARTITION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename Predicate, typename ForwardIterator>
    auto partition_impl(ForwardIterator first, ForwardIterator last, Predicate pred,
                        std::forward_iterator_tag)
        -> ForwardIterator
    {
        while (true)
        {
            if (first == last)
                return first;
            if (!pred(*first))
                break;
            ++first;
        }
        for (ForwardIterator p = first; ++p != last;)
        {
            if (pred(*p))
            {
                using utility::iter_swap;
                iter_swap(first, p);
                ++first;
            }
        }
        return first;
    }

    template<typename Predicate, typename BidirectionalIterator>
    auto partition_impl(BidirectionalIterator first, BidirectionalIterator last, Predicate pred,
                        std::bidirectional_iterator_tag)
        -> BidirectionalIterator
    {
        while (true)
        {
            while (true)
            {
                if (first == last)
                    return first;
                if (!pred(*first))
                    break;
                ++first;
            }
            do
            {
                if (first == --last)
                    return first;
            } while (!pred(*last));
            using utility::iter_swap;
            iter_swap(first, last);
            ++first;
        }
    }

    template<typename ForwardIterator, typename Predicate>
    auto partition(ForwardIterator first, ForwardIterator last, Predicate pred)
        -> ForwardIterator
    {
        return partition_impl<std::add_lvalue_reference_t<Predicate>>(
            std::move(first), std::move(last), pred,
            iterator_category_t<ForwardIterator>{}
        );
    }
}}

#endif // CPPSORT_DETAIL_PARTITION_H_
