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
#ifndef CPPSORT_DETAIL_QUICKSORT_H_
#define CPPSORT_DETAIL_QUICKSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include "bubble_sort.h"
#include "insertion_sort.h"
#include "iter_sort3.h"

namespace cppsort
{
namespace detail
{
    template <typename ForwardIterator, typename Compare>
    void quicksort(ForwardIterator first, ForwardIterator last,
                   Compare compare, std::size_t size,
                   std::forward_iterator_tag category)
    {
        // If the collection is small, fall back to
        // bubble sort
        if (size < 10)
        {
            bubble_sort(first, compare, size);
            return;
        }

        // Choose pivot as median of 3
        ForwardIterator middle = std::next(first, size / 2);
        iter_sort3(first, middle,
                   std::next(middle, size - size/2 - 1),
                   compare);
        const auto& pivot = *middle;

        // Partition the collection
        ForwardIterator middle1 = std::partition(
            first, last,
            [=](const auto& elem) { return compare(elem, pivot); }
        );
        ForwardIterator middle2 = std::partition(
            middle1, last,
            [=](const auto& elem) { return not compare(pivot, elem); }
        );

        // Recursive call: heuristic trick here
        // The "middle" partition is more likely to be smaller than the
        // last one, so computing its size should generally be cheaper
        std::size_t size_left = std::distance(first, middle1);
        quicksort(first, middle1, compare, size_left, category);
        quicksort(middle2, last, compare,
                  size - size_left - std::distance(middle1, middle2),
                  category);
    }

    template <typename BidirectionalIterator, typename Compare>
    void quicksort(BidirectionalIterator first, BidirectionalIterator last,
                   Compare compare, std::size_t size,
                   std::bidirectional_iterator_tag category)
    {
        // If the collection is small, fall back to
        // insertion sort
        if (size < 42)
        {
            insertion_sort(first, last, compare);
            return;
        }

        // Choose pivot as median of 3
        BidirectionalIterator middle = std::next(first, size / 2);
        iter_sort3(first, middle, std::prev(last), compare);
        const auto& pivot = *middle;

        // Partition the collection
        BidirectionalIterator middle1 = std::partition(
            first, last,
            [=](const auto& elem) { return compare(elem, pivot); }
        );
        BidirectionalIterator middle2 = std::partition(
            middle1, last,
            [=](const auto& elem) { return not compare(pivot, elem); }
        );

        // Recursive call: heuristic trick here
        // The "middle" partition is more likely to be smaller than the
        // last one, so computing its size should generally be cheaper
        std::size_t size_left = std::distance(first, middle1);
        quicksort(first, middle1, compare, size_left, category);
        quicksort(middle2, last, compare,
                  size - size_left - std::distance(middle1, middle2),
                  category);
    }

    template <typename ForwardIterator, typename Compare>
    void quicksort(ForwardIterator first, ForwardIterator last,
                   Compare compare, std::size_t size)
    {
        using category = typename std::iterator_traits<ForwardIterator>::iterator_category;
        quicksort(first, last, compare, size, category{});
    }
}}

#endif // CPPSORT_DETAIL_QUICKSORT_H_
