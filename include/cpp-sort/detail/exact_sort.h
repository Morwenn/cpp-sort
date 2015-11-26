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
	inline auto real_index(const std::vector<bool>& sorted, std::size_t n)
        -> int
	{
	    // Number of encountered elements not already sorted
		std::size_t unsorted = 0;

		for (std::size_t i = 0 ; i < sorted.size() ; ++i)
		{
			if (not sorted[i])
            {
				++unsorted;
            }

			if (unsorted == n + 1)
            {
				return i;
            }
		}
		return -1;
	}

    // Returns the index of the first element in the collection
    // that hasn't been sorted yet
	inline auto first_false(const std::vector<bool>& sorted)
        -> int
	{
        return real_index(sorted, 0);
	}

    // Returns the destination of the given value, where the destination
    // corresponds to the final position of the given value once the whole
    // collection has been sorted
    template<typename RandomAccessIterator, typename T,
             typename Compare, typename Projection>
	auto get_destination(RandomAccessIterator first, RandomAccessIterator last,
                         Compare compare, Projection projection,
                         const std::vector<bool>& sorted,
                         const T& value, int start)
        -> int
	{
	    auto&& proj = utility::as_function(projection);

        // Number of unsorted elements smaller elements than value
		std::size_t count = 0;

		for (int i = 0 ; i < std::distance(first, last) ; ++i)
		{
			if (not sorted[i] && compare(proj(first[i]), proj(value)) && i != start)
            {
				++count;
            }
		}
        return real_index(sorted, count);
	}

	template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto exact_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        if (first == last) return;

        // Which elements are already sorted, and which ones still
        // need to be sorted
        std::vector<bool> sorted(std::distance(first, last), false);

        // Position of the element where the current cycle starts
		int start = 0;

		// Stack of positions whose top corresponds to the position
		// of the current element
		std::stack<int> positions;

		while (true)
		{
            int dest; // Final destination of the current element
            if (positions.empty())
            {
                dest = get_destination(first, last, compare, projection, sorted,
                                       first[start], start);
            }
            else
            {
                dest = get_destination(first, last, compare, projection, sorted,
                                       first[positions.top()], start);
            }

            // There is nothing else to sort
			if (dest == -1) return;

			// Mark the destination as "sorted"
			sorted[dest] = true;

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
                    auto tmp = std::move(first[dest]);
                    while (not positions.empty())
                    {
                        first[dest] = std::move(first[positions.top()]);
                        dest = positions.top();
                        positions.pop();
                    }
                    first[dest] = std::move(tmp);
                }

                // The next cycle starts at the first unsorted element
                // of the collection
				start = first_false(sorted);

                // If there is no such element, it means that the collection
                // is already sorted
                if (start == -1) return;
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
