/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_DETAIL_COUNT_INVERSIONS_H_
#define CPPSORT_DETAIL_COUNT_INVERSIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <memory>

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare>
    auto count_inversions_merge(RandomAccessIterator first, RandomAccessIterator middle,
                                RandomAccessIterator last, Compare compare,
                                typename std::iterator_traits<RandomAccessIterator>::difference_type size)
        -> typename std::iterator_traits<RandomAccessIterator>::difference_type
    {
        using value_type = typename std::iterator_traits<RandomAccessIterator>::value_type;
        using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;

        std::unique_ptr<value_type[]> buffer(std::make_unique<value_type[]>(size));
        difference_type inversions = 0;

        auto result = buffer.get();
        auto first2 = middle;
        for (auto first1 = first ; first1 != middle ; ++result)
        {
            if (first2 == last)
            {
                std::move(first1, middle, result);
                break;
            }
            if (compare(*first2, *first1))
            {
                *result = std::move(*first2);
                ++first2;

                inversions += std::distance(first1, middle);
            }
            else
            {
                *result = std::move(*first1);
                ++first1;
            }
        }
        std::move(first2, last, result);

        // Move everything back to the original array
        std::move(buffer.get(), buffer.get() + size, first);
        return inversions;
    }

    template<typename RandomAccessIterator, typename Compare>
    auto count_inversions(RandomAccessIterator first, RandomAccessIterator last,
                          Compare compare,
                          typename std::iterator_traits<RandomAccessIterator>::difference_type size)
        -> typename std::iterator_traits<RandomAccessIterator>::difference_type
    {
        using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;

        if (size < 2)
        {
            return 0;
        }

        difference_type inversions = 0;
        auto dist = size / 2;
        auto middle = std::next(first, dist);

        inversions += count_inversions(first, middle, compare, dist);
        inversions += count_inversions(middle, last, compare, size - dist);
        inversions += count_inversions_merge(first, middle, last, compare, size);
        return inversions;
    }
}}

#endif // CPPSORT_DETAIL_COUNT_INVERSIONS_H_
