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
#ifndef CPPSORT_DETAIL_SPLIT_SORT_H_
#define CPPSORT_DETAIL_SPLIT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <vector>
#include "inplace_merge.h"
#include "iterator_traits.h"
#include "pdqsort.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto split_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        if (std::distance(first, last) < 2) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Read and reorganize elements until middle is found
        auto middle = first; // Last element of the LAS
        for (auto reader_it = std::next(first) ; reader_it != last ; ++reader_it) {
            if (comp(proj(*reader_it), proj(*middle))) {
                // We remove the top of the subsequence as well as the new element
                if (middle != first) {
                    --middle;
                }
            } else {
                // Everything is fine, add the new element to the subsequence
                ++middle;
                if (middle != reader_it) {
                    iter_swap(middle, reader_it);
                }
            }
        }

        // Sort second part of the collection and merge
        pdq_sort(middle, last, compare, projection);
        inplace_merge(first, middle, last, compare, projection);
    }
}}

#endif // CPPSORT_DETAIL_SPLIT_SORT_H_
