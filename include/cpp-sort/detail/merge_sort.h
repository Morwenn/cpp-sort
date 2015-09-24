/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_DETAIL_MERGE_SORT_H_
#define CPPSORT_DETAIL_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <cpp-sort/utility/inplace_merge.h>

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare>
    void merge_sort(ForwardIterator first, ForwardIterator last,
                    Compare compare, std::size_t size)
    {
        if (size < 2) return;

        // Divide the range into two partitions
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively sort the partitions
        merge_sort(first, middle, compare, size_left);
        merge_sort(middle, last, compare, size - size_left);

        // Merge the sorted partitions in-place
        utility::inplace_merge(first, middle, last, compare);
    }
}}

#endif // CPPSORT_DETAIL_MERGE_SORT_H_
