/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_RECMERGE_BIDIRECTIONAL_H_
#define CPPSORT_DETAIL_RECMERGE_BIDIRECTIONAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "buffered_inplace_merge.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "rotate.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // recmerge for bidirectional iterators

    template<typename BidirectionalIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto recmerge(BidirectionalIterator first, BidirectionalIterator middle,
                  BidirectionalIterator last,
                  Compare compare, Projection projection,
                  difference_type_t<BidirectionalIterator> len1,
                  difference_type_t<BidirectionalIterator> len2,
                  RandomAccessIterator buff, std::ptrdiff_t buff_size,
                  std::bidirectional_iterator_tag tag)
        -> void
    {
        using difference_type = difference_type_t<BidirectionalIterator>;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (true) {
            // if middle == last, we're done
            if (len2 == 0)
                return;
            if (len1 <= buff_size || len2 <= buff_size) {
                buffered_inplace_merge(first, middle, last,
                                       std::move(compare), std::move(projection),
                                       len1, len2, buff);
                return;
            }
            // shrink [first, middle) as much as possible (with no moves), returning if it shrinks to 0
            for (; true; ++first, (void) --len1) {
                if (len1 == 0)
                    return;
                if (comp(proj(*middle), proj(*first)))
                    break;
            }
            // first < middle < last
            // *first > *middle
            // partition [first, m1) [m1, middle) [middle, m2) [m2, last) such that
            //     all elements in:
            //         [first, m1)  <= [middle, m2)
            //         [middle, m2) <  [m1, middle)
            //         [m1, middle) <= [m2, last)
            //     and m1 or m2 is in the middle of its range
            BidirectionalIterator m1;  // "median" of [first, middle)
            BidirectionalIterator m2;  // "median" of [middle, last)
            difference_type len11;      // distance(first, m1)
            difference_type len21;      // distance(middle, m2)
            // binary search smaller range
            if (len1 < len2) {
                // len >= 1, len2 >= 2
                len21 = len2 / 2;
                m2 = middle;
                std::advance(m2, len21);
                m1 = upper_bound(first, middle, proj(*m2), compare, projection);
                len11 = std::distance(first, m1);
            } else {
                if (len1 == 1) {
                    // len1 >= len2 && len2 > 0, therefore len2 == 1
                    // It is known *first > *middle
                    using utility::iter_swap;
                    iter_swap(first, middle);
                    return;
                }
                // len1 >= 2, len2 >= 1
                len11 = len1 / 2;
                m1 = first;
                std::advance(m1, len11);
                m2 = lower_bound(middle, last, proj(*m1), compare, projection);
                len21 = std::distance(middle, m2);
            }
            difference_type len12 = len1 - len11;  // distance(m1, middle)
            difference_type len22 = len2 - len21;  // distance(m2, last)
            // [first, m1) [m1, middle) [middle, m2) [m2, last)
            // swap middle two partitions
            middle = detail::rotate(m1, middle, m2);
            // len12 and len21 now have swapped meanings
            // merge smaller range with recursive call and larger with tail recursion elimination
            if (len11 + len21 < len12 + len22) {
                recmerge(first, m1, middle, compare, projection,
                         len11, len21, buff, buff_size, tag);
//              recmerge_impl(middle, m2, last, compare, projection,
//                            len12, len22, buff, buff_size);
                first = middle;
                middle = m2;
                len1 = len12;
                len2 = len22;
            } else {
                recmerge(middle, m2, last, compare, projection,
                         len12, len22, buff, buff_size, tag);
//              recmerge_impl(first, m1, middle, compare, projection,
//                            len11, len21, buff, buff_size);
                last = middle;
                middle = m1;
                len1 = len11;
                len2 = len21;
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_RECMERGE_BIDIRECTIONAL_H_
