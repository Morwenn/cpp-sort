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

#ifndef CPPSORT_DETAIL_VERGESORT_H_
#define CPPSORT_DETAIL_VERGESORT_H_

#include <algorithm>
#include <iterator>
#include "pdqsort.h"

namespace cppsort
{
namespace detail
{
    // In-place merge where [first, middle1), [middle1, middle2)
    // and [middle2, last) are sorted. The two in-place merges are
    // done in the order that should result in the smallest number
    // of comparisons
    template<typename RandomAccessIterator, typename Compare>
    void inplace_merge3(RandomAccessIterator first,
                        RandomAccessIterator middle1,
                        RandomAccessIterator middle2,
                        RandomAccessIterator last,
                        Compare compare)
    {
        if (std::distance(first, middle1) < std::distance(middle2, last))
        {
            std::inplace_merge(first, middle1, middle2, compare);
            std::inplace_merge(first, middle2, last, compare);
        }
        else
        {
            std::inplace_merge(middle1, middle2, last, compare);
            std::inplace_merge(first, middle1, last, compare);
        }
    }

    template<typename RandomAccessIterator, typename Compare>
    void vergesort(RandomAccessIterator first, RandomAccessIterator last, Compare compare)
    {
        typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
        difference_type dist = std::distance(first, last);

        if (dist < 80)
        {
            // vergesort is inefficient for small collections
            pdqsort(first, last, compare);
            return;
        }

        // Limit under which pdqsort is used
        difference_type unstable_limit = dist / pdqsort_detail::log2(dist);

        // Beginning of an unstable partition, last if the
        // previous partition is stable
        RandomAccessIterator begin_unstable = last;

        // Pair of iterators to iterate through the collection
        RandomAccessIterator current = std::is_sorted_until(first, last, compare) - 1;
        RandomAccessIterator next = current + 1;

        while (true)
        {
            // Beginning of the current range
            RandomAccessIterator begin_range = current;

            // Set backward iterators
            difference_type limit = std::min(std::distance(next, last), unstable_limit);
            std::advance(current, limit);
            std::advance(next, limit);

            // Set forward iterators
            RandomAccessIterator current2 = current;
            RandomAccessIterator next2 = next;

            if (compare(*current, *next))
            {
                // Found an increasing range, move iterators
                // to the limits of the range
                while (current != begin_range)
                {
                    --current;
                    --next;
                    if (compare(*next, *current)) break;
                }
                if (compare(*next, *current)) ++current;

                while (next2 != last)
                {
                    if (compare(*next2, *current2)) break;
                    ++current2;
                    ++next2;
                }

                // Remember the beginning of the unsorted range
                if (begin_unstable == last) begin_unstable = begin_range;

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit)
                {
                    pdqsort(begin_unstable, current, compare);
                    inplace_merge3(first, begin_unstable, current, next2, compare);
                    begin_unstable = last;
                }
            }
            else
            {
                // Found an decreasing range, move iterators
                // to the limits of the range
                while (current != begin_range)
                {
                    --current;
                    --next;
                    if (compare(*current, *next)) break;
                }
                if (compare(*current, *next)) ++current;

                while (next2 != last)
                {
                    if (compare(*current2, *next2)) break;
                    ++current2;
                    ++next2;
                }

                // Remember the beginning of the unsorted range
                if (begin_unstable == last) begin_unstable = begin_range;

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit)
                {
                    pdqsort(begin_unstable, current, compare);
                    std::reverse(current, next2);
                    inplace_merge3(first, begin_unstable, current, next2, compare);
                    begin_unstable = last;
                }
            }

            if (next2 == last) break;

            current = current2 + 1;
            next = next2 + 1;
        }

        if (begin_unstable != last)
        {
            // If there are unsorted elements left,
            // sort them and merge everything
            pdqsort(begin_unstable, last, compare);
            std::inplace_merge(first, begin_unstable, last, compare);
        }
    }
}}

#endif // CPPSORT_DETAIL_VERGESORT_H_
