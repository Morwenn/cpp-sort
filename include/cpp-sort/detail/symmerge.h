/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018-2020 Morwenn
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
#ifndef CPPSORT_DETAIL_SYMMERGE_H_
#define CPPSORT_DETAIL_SYMMERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "buffered_inplace_merge.h"
#include "iterator_traits.h"
#include "rotate.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Random-access algorithm using symmerge

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto symmerge_bsearch(RandomAccessIterator arr,
                          difference_type_t<RandomAccessIterator> l,
                          difference_type_t<RandomAccessIterator> r,
                          difference_type_t<RandomAccessIterator> p,
                          Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
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
                  difference_type_t<RandomAccessIterator1> first,
                  difference_type_t<RandomAccessIterator1> middle,
                  difference_type_t<RandomAccessIterator1> last,
                  Compare compare, Projection projection,
                  difference_type_t<RandomAccessIterator1> len1,
                  difference_type_t<RandomAccessIterator1> len2,
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
}}

#endif // CPPSORT_DETAIL_SYMMERGE_H_
