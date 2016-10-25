/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/bitops.h>
#include "inplace_merge.h"
#include "is_sorted_until.h"
#include "iterator_traits.h"
#include "pdqsort.h"
#include "quicksort.h"
#include "reverse.h"

namespace cppsort
{
namespace detail
{
    // In-place merge where [first, middle1), [middle1, middle2)
    // and [middle2, last) are sorted. The two in-place merges are
    // done in the order that should result in the smallest number
    // of comparisons
    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge3(BidirectionalIterator first, BidirectionalIterator middle1,
                        BidirectionalIterator middle2, BidirectionalIterator last,
                        Compare compare, Projection projection)
        -> void
    {
        if (std::distance(first, middle1) < std::distance(middle2, last))
        {
            detail::inplace_merge(first, middle1, middle2, compare, projection);
            detail::inplace_merge(first, middle2, last,
                                  std::move(compare), std::move(projection));
        }
        else
        {
            detail::inplace_merge(middle1, middle2, last, compare, projection);
            detail::inplace_merge(first, middle1, last,
                                  std::move(compare), std::move(projection));
        }
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto vergesort(BidirectionalIterator first, BidirectionalIterator last,
                   Compare compare, Projection projection,
                   std::bidirectional_iterator_tag)
        -> void
    {
        using difference_type = difference_type_t<BidirectionalIterator>;
        difference_type dist = std::distance(first, last);

        if (dist < 80)
        {
            // vergesort is inefficient for small collections
            quicksort(std::move(first), std::move(last), dist,
                      std::move(compare), std::move(projection));
            return;
        }

        // Limit under which quicksort is used
        int unstable_limit = dist / utility::log2(dist);

        // Beginning of an unstable partition, last if the
        // previous partition is stable
        BidirectionalIterator begin_unstable = last;

        // Size of the unstable partition
        difference_type size_unstable = 0;

        // Pair of iterators to iterate through the collection
        BidirectionalIterator next = is_sorted_until(first, last, compare, projection);
        BidirectionalIterator current = std::prev(next);

        auto&& proj = utility::as_function(projection);

        while (true)
        {
            BidirectionalIterator begin_rng = current;

            // Decreasing range
            while (next != last)
            {
                if (compare(proj(*current), proj(*next))) break;
                ++current;
                ++next;
            }

            // Reverse and merge
            dist = std::distance(begin_rng, next);
            if (dist > unstable_limit)
            {
                if (begin_unstable != last)
                {
                    quicksort(begin_unstable, begin_rng, size_unstable, compare, projection);
                    detail::reverse(begin_rng, next);
                    detail::inplace_merge(begin_unstable, begin_rng, next, compare, projection);
                    detail::inplace_merge(first, begin_unstable, next, compare, projection);
                    begin_unstable = last;
                    size_unstable = 0;
                }
                else
                {
                    detail::reverse(begin_rng, next);
                    detail::inplace_merge(first, begin_rng, next, compare, projection);
                }
            }
            else
            {
                size_unstable += dist;
                if (begin_unstable == last) begin_unstable = begin_rng;
            }

            if (next == last) break;

            ++current;
            ++next;

            begin_rng = current;

            // Increasing range
            while (next != last)
            {
                if (compare(proj(*next), proj(*current))) break;
                ++current;
                ++next;
            }

            // Merge
            dist = std::distance(begin_rng, next);
            if (dist > unstable_limit)
            {
                if (begin_unstable != last)
                {
                    quicksort(begin_unstable, begin_rng, size_unstable, compare, projection);
                    detail::inplace_merge(begin_unstable, begin_rng, next, compare, projection);
                    detail::inplace_merge(first, begin_unstable, next, compare, projection);
                    begin_unstable = last;
                    size_unstable = 0;
                }
                else
                {
                    detail::inplace_merge(first, begin_rng, next, compare, projection);
                }
            }
            else
            {
                size_unstable += dist;
                if (begin_unstable == last) begin_unstable = begin_rng;
            }

            if (next == last) break;

            ++current;
            ++next;
        }

        if (begin_unstable != last)
        {
            quicksort(begin_unstable, last, size_unstable, compare, projection);
            detail::inplace_merge(first, begin_unstable, last,
                                  std::move(compare), std::move(projection));
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto vergesort(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection,
                   std::random_access_iterator_tag)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        difference_type dist = std::distance(first, last);

        if (dist < 80)
        {
            // vergesort is inefficient for small collections
            pdqsort(std::move(first), std::move(last),
                    std::move(compare), std::move(projection));
            return;
        }

        // Limit under which pdqsort is used
        difference_type unstable_limit = dist / utility::log2(dist);

        // Beginning of an unstable partition, last if the
        // previous partition is stable
        RandomAccessIterator begin_unstable = last;

        // Pair of iterators to iterate through the collection
        RandomAccessIterator next = is_sorted_until(first, last, compare, projection);
        if (next == last) return;
        RandomAccessIterator current = std::prev(next);

        auto&& proj = utility::as_function(projection);

        while (true)
        {
            // Beginning of the current range
            RandomAccessIterator begin_range = current;

            if (std::distance(next, last) <= unstable_limit)
            {
                if (begin_unstable == last)
                {
                    begin_unstable = begin_range;
                }
                break;
            }

            // Set backward iterators
            std::advance(current, unstable_limit);
            std::advance(next, unstable_limit);

            // Set forward iterators
            RandomAccessIterator current2 = current;
            RandomAccessIterator next2 = next;

            if (compare(proj(*current), proj(*next)))
            {
                // Found an increasing range, move iterators
                // to the limits of the range
                while (current != begin_range)
                {
                    --current;
                    --next;
                    if (compare(proj(*next), proj(*current))) break;
                }
                if (compare(proj(*next), proj(*current))) ++current;

                while (next2 != last)
                {
                    if (compare(proj(*next2), proj(*current2))) break;
                    ++current2;
                    ++next2;
                }

                // Remember the beginning of the unsorted range
                if (begin_unstable == last) begin_unstable = begin_range;

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit)
                {
                    pdqsort(begin_unstable, current, compare, projection);
                    inplace_merge3(first, begin_unstable, current, next2, compare, projection);
                    begin_unstable = last;
                }
            }
            else
            {
                // Found a decreasing range, move iterators
                // to the limits of the range
                while (current != begin_range)
                {
                    --current;
                    --next;
                    if (compare(proj(*current), proj(*next))) break;
                }
                if (compare(proj(*current), proj(*next))) ++current;

                while (next2 != last)
                {
                    if (compare(proj(*current2), proj(*next2))) break;
                    ++current2;
                    ++next2;
                }

                // Remember the beginning of the unsorted range
                if (begin_unstable == last) begin_unstable = begin_range;

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit)
                {
                    pdqsort(begin_unstable, current, compare, projection);
                    detail::reverse(current, next2);
                    inplace_merge3(first, begin_unstable, current, next2, compare, projection);
                    begin_unstable = last;
                }
            }

            if (next2 == last) break;

            current = std::next(current2);
            next = std::next(next2);
        }

        if (begin_unstable != last)
        {
            // If there are unsorted elements left,
            // sort them and merge everything
            pdqsort(begin_unstable, last, compare, projection);
            detail::inplace_merge(first, begin_unstable, last,
                                  std::move(compare), std::move(projection));
        }
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto vergesort(BidirectionalIterator first, BidirectionalIterator last,
                   Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<BidirectionalIterator>;
        vergesort(std::move(first), std::move(last),
                  std::move(compare), std::move(projection),
                  category{});
    }
}}

#endif // CPPSORT_DETAIL_VERGESORT_H_
