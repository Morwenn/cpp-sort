/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#include <list>
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

        if (dist < 80) {
            // Vergesort is inefficient for small collections
            pdqsort(std::move(first), std::move(last),
                    std::move(compare), std::move(projection));
            return;
        }

        // Limit under which pdqsort is used to sort a sub-sequence
        const difference_type unstable_limit = dist / utility::log2(dist);

        // Vergesort detects big runs in ascending or descending order,
        // and remember where each run ends by storing the end iterator
        // of each run in this list, then it merges everything in the end
        std::list<RandomAccessIterator> runs;

        // Beginning of an unstable partition, or last if the previous
        // partition is stable
        RandomAccessIterator begin_unstable = last;

        // Pair of iterators to iterate through the collection
        RandomAccessIterator current = first;
        RandomAccessIterator next = std::next(first);

        auto&& proj = utility::as_function(projection);

        while (true) {
            // Beginning of the current sequence
            RandomAccessIterator begin_range = current;

            // If the last part of the collection to sort isn't
            // big enough, consider that it is an unstable sequence
            if (std::distance(next, last) <= unstable_limit) {
                if (begin_unstable == last) {
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

            if (compare(proj(*next), proj(*current))) {
                // Found a decreasing sequence, move iterators
                // to the limits of the sequence
                while (current != begin_range) {
                    --current;
                    --next;
                    if (compare(proj(*current), proj(*next))) break;
                }
                if (compare(proj(*current), proj(*next))) ++current;

                ++current2;
                ++next2;
                while (next2 != last) {
                    if (compare(proj(*current2), proj(*next2))) break;
                    ++current2;
                    ++next2;
                }

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit) {
                    detail::reverse(current, next2);
                    if (std::distance(begin_range, current) && begin_unstable == last) {
                        begin_unstable = begin_range;
                    }
                    if (begin_unstable != last) {
                        pdqsort(begin_unstable, current, compare, projection);
                        runs.push_back(current);
                        begin_unstable = last;
                    }
                    runs.push_back(next2);
                } else {
                    // Remember the beginning of the unsorted sequence
                    if (begin_unstable == last) {
                        begin_unstable = begin_range;
                    }
                }
            } else {
                // Found an increasing sequence, move iterators
                // to the limits of the sequence
                while (current != begin_range) {
                    --current;
                    --next;
                    if (compare(proj(*next), proj(*current))) break;
                }
                if (compare(proj(*next), proj(*current))) ++current;

                ++current2;
                ++next2;
                while (next2 != last) {
                    if (compare(proj(*next2), proj(*current2))) break;
                    ++current2;
                    ++next2;
                }

                // Check whether we found a big enough sorted sequence
                if (std::distance(current, next2) >= unstable_limit) {
                    if (std::distance(begin_range, current) && begin_unstable == last) {
                        begin_unstable = begin_range;
                    }
                    if (begin_unstable != last) {
                        pdqsort(begin_unstable, current, compare, projection);
                        runs.push_back(current);
                        begin_unstable = last;
                    }
                    runs.push_back(next2);
                } else {
                    // Remember the beginning of the unsorted sequence
                    if (begin_unstable == last) {
                        begin_unstable = begin_range;
                    }
                }
            }

            if (next2 == last) break;

            current = std::next(current2);
            next = std::next(next2);
        }

        if (begin_unstable != last) {
            // If there are unsorted elements left, sort them
            runs.push_back(last);
            pdqsort(begin_unstable, last, compare, projection);
        }

        if (runs.size() < 2) return;

        // Merge runs pairwise until there aren't runs left
        do {
            auto begin = first;
            for (auto it = runs.begin() ; it != runs.end() && it != std::prev(runs.end()) ; ++it) {
                detail::inplace_merge(begin, *it, *std::next(it),
                                      compare, projection);

                // Remove the middle iterator and advance
                it = runs.erase(it);
                begin = *it;
            }
        } while (runs.size() > 1);
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
