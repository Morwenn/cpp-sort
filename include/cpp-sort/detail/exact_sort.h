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
#ifndef CPPSORT_DETAIL_EXACT_SORT_H_
#define CPPSORT_DETAIL_EXACT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <stack>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    // Returns the index of the position of the nth element of the
    // collection not already in its place
    template<typename RandomAccessIterator>
    auto real_index(RandomAccessIterator first, RandomAccessIterator last,
                    const std::vector<bool>& sorted, std::size_t n)
        -> RandomAccessIterator
    {
        // Number of encountered elements not already sorted
        std::size_t unsorted = 0;

        for (std::size_t i = 0 ; i < sorted.size() ; ++i)
        {
            if (not sorted[i])
            {
                if (unsorted == n)
                {
                    return std::next(first, i);
                }
                ++unsorted;
            }
        }
        return last;
    }

    // Returns the index of the first element in the collection
    // that hasn't been sorted yet
    template<typename RandomAccessIterator>
    auto first_false(RandomAccessIterator first, RandomAccessIterator last,
                     const std::vector<bool>& sorted)
        -> RandomAccessIterator
    {
        return real_index(first, last, sorted, 0);
    }

    // Returns the destination of the given value, where the destination
    // corresponds to the final position of the given value once the whole
    // collection has been sorted
    template<typename RandomAccessIterator, typename T,
             typename Compare, typename Projection>
    auto get_destination(RandomAccessIterator first, RandomAccessIterator last,
                         Compare compare, Projection projection,
                         const std::vector<bool>& sorted,
                         const T& value, RandomAccessIterator start)
        -> RandomAccessIterator
    {
        auto&& proj = utility::as_function(projection);

        // Number of unsorted elements smaller than value
        std::size_t count = 0;

        for (auto it = first ; it != last ; ++it)
        {
            if (not sorted[it - first] && compare(proj(*it), value) && it != start)
            {
                ++count;
            }
        }
        return real_index(first, last, sorted, count);
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto exact_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        if (first == last) return;

        auto&& proj = utility::as_function(projection);

        // Which elements are already sorted, and which ones still
        // need to be sorted
        std::vector<bool> sorted(std::distance(first, last), false);

        // Element where the current cycle starts
        RandomAccessIterator start = first;

        // Stack of elements, top is the current element
        std::stack<
            RandomAccessIterator,
            std::vector<RandomAccessIterator>
        > positions;

        while (true)
        {
            const auto& value = positions.empty() ? *start : *positions.top();
            RandomAccessIterator dest = get_destination(first, last, compare, projection,
                                                        sorted, proj(value), start);

            // There is nothing else to sort
            if (dest == last) return;

            // Mark the destination as "sorted"
            sorted[dest - first] = true;

            // When the beginning of the current cycle is the same as the
            // destination of the element to sort, we have reached the end
            // of the cycle
            if (dest == start)
            {
                // If the stack is empty, it means that the starting point
                // is already in its final position, do nothing

                if (not positions.empty())
                {
                    // Move elements to their final positions
                    auto tmp = std::move(*dest);
                    while (not positions.empty())
                    {
                        *dest = std::move(*positions.top());
                        dest = positions.top();
                        positions.pop();
                    }
                    *dest = std::move(tmp);
                }

                // The next cycle starts at the first unsorted element
                // of the collection
                start = first_false(first, last, sorted);

                // If there is no such element, it means that the collection
                // is already sorted
                if (start == last) return;
            }
            else
            {
                // Push the destination on the top of the stack
                positions.push(dest);
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_EXACT_SORT_H_
