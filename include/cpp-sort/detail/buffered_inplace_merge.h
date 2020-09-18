/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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

#ifndef CPPSORT_DETAIL_BUFFERED_INPLACE_MERGE_H_
#define CPPSORT_DETAIL_BUFFERED_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "iterator_traits.h"
#include "memory.h"
#include "move.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Blind merge with a buffer

    template<typename InputIterator1, typename InputIterator2,
             typename OutputIterator, typename Size,
             typename Compare, typename Projection>
    auto half_inplace_merge(InputIterator1 first1, InputIterator1 last1,
                            InputIterator2 first2, InputIterator2 last2,
                            OutputIterator result, Size min_len,
                            Compare compare, Projection projection)
        -> void
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (; min_len != 0 ; --min_len) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);
            if (comp(proj(*first2), proj(*first1))) {
                *result = iter_move(first2);
                ++first2;
            } else {
                *result = iter_move(first1);
                ++first1;
            }
            ++result;
        }

        if (first1 == last1) {
            // first2 through last2 are already in the right spot
            return;
        }

        if (first2 == last2) {
            detail::move(first1, last1, result);
            return;
        }

        while (true) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);

            if (comp(proj(*first2), proj(*first1))) {
                *result = iter_move(first2);
                ++first2;
                if (first2 == last2) {
                    detail::move(first1, last1, ++result);
                    return;
                }
            } else {
                *result = iter_move(first1);
                ++first1;
                if (first1 == last1) {
                    // first2 through last2 are already in the right spot
                    return;
                }
            }
            ++result;
        }
    }

    ////////////////////////////////////////////////////////////
    // Prepare the buffer prior to the blind merge (only for
    // bidirectional iterator)

    template<typename Predicate>
    class invert
    {
        private:

            Predicate predicate;

        public:

            invert() {}

            explicit invert(Predicate predicate):
                predicate(std::move(predicate))
            {}

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y)
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return pred(std::forward<T2>(y), std::forward<T1>(x));
            }

            template<typename T1, typename T2>
            auto operator()(T1&& x, T2&& y) const
                -> bool
            {
                auto&& pred = utility::as_function(predicate);
                return pred(std::forward<T2>(y), std::forward<T1>(x));
            }
    };

    template<typename BidirectionalIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto buffered_inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                                BidirectionalIterator last,
                                Compare compare, Projection projection,
                                difference_type_t<BidirectionalIterator> len1,
                                difference_type_t<BidirectionalIterator> len2,
                                RandomAccessIterator buff)
        -> void
    {
        using utility::iter_move;
        using rvalue_reference = remove_cvref_t<rvalue_reference_t<BidirectionalIterator>>;
        destruct_n<rvalue_reference> d(0);
        std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buff, d);
        if (len1 <= len2) {
            auto ptr = uninitialized_move(first, middle, buff, d);
            half_inplace_merge(buff, ptr, middle, last, first, len1,
                               std::move(compare), std::move(projection));
        } else {
            auto ptr = uninitialized_move(middle, last, buff, d);
            using rbi = std::reverse_iterator<BidirectionalIterator>;
            using rv = std::reverse_iterator<rvalue_reference*>;
            half_inplace_merge(rv(ptr), rv(buff),
                               rbi(middle), rbi(first),
                               rbi(last), len2,
                               invert<Compare>(compare), std::move(projection));
        }
    }
}}

#endif // CPPSORT_DETAIL_BUFFERED_INPLACE_MERGE_H_
