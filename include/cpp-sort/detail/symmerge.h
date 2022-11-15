/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SYMMERGE_H_
#define CPPSORT_DETAIL_SYMMERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/utility/as_function.h>
#include "buffered_inplace_merge.h"
#include "rotate.h"

namespace cppsort::detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto symmerge_bsearch(RandomAccessIterator arr,
                          mstd::iter_difference_t<RandomAccessIterator> l,
                          mstd::iter_difference_t<RandomAccessIterator> r,
                          mstd::iter_difference_t<RandomAccessIterator> p,
                          Compare compare, Projection projection)
        -> mstd::iter_difference_t<RandomAccessIterator>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (l < r) {
            auto m = l + (r - l) / 2;
            if (not comp(proj(arr[p-m]), proj(arr[m]))) {
                l = m + 1;
            } else {
                r = m;
            }
        }
        return l;
    }

    template<typename RandomAccessIterator1, typename RandomAccessIterator2,
             typename Compare, typename Projection>
    auto symmerge(RandomAccessIterator1 arr,
                  mstd::iter_difference_t<RandomAccessIterator1> first,
                  mstd::iter_difference_t<RandomAccessIterator1> middle,
                  mstd::iter_difference_t<RandomAccessIterator1> last,
                  Compare compare, Projection projection,
                  mstd::iter_difference_t<RandomAccessIterator1> len1,
                  mstd::iter_difference_t<RandomAccessIterator1> len2,
                  RandomAccessIterator2 buff, std::ptrdiff_t buff_size)
        -> void
    {
        while (len1 > 0 && len2 > 0)  {

            if (len1 <= buff_size || len2 <= buff_size) {
                buffered_inplace_merge(arr + first, arr + middle, arr + last,
                                       std::move(compare), std::move(projection),
                                       len1, len2, buff);
                return;
            }

            auto m = first + (last - first) / 2;
            auto n = m + middle;
            auto start = middle > m ?
                symmerge_bsearch(arr, n - last, m, n - 1, compare, projection) :
                symmerge_bsearch(arr, first, middle, n - 1, compare, projection);
            auto end = n - start;

            detail::rotate(arr + start, arr + middle, arr + end);
            symmerge(arr, first, start, m, compare, projection,
                     start - first, m - start, buff, buff_size);

            // Unrolled tail recursion, last doesn't change
            first = m;
            middle = end;
            len1 = middle - first;
            len2 = last - middle;
        }
    }
}

#endif // CPPSORT_DETAIL_SYMMERGE_H_
