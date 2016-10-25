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
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "move.h"

namespace cppsort
{
namespace detail
{
    template<
        typename ResultType,
        typename RandomAccessIterator1,
        typename RandomAccessIterator2,
        typename Compare
    >
    auto count_inversions_merge(RandomAccessIterator1 first, RandomAccessIterator1 middle,
                                RandomAccessIterator1 last, RandomAccessIterator2 cache,
                                Compare compare)
        -> ResultType
    {
        using utility::iter_move;

        ResultType inversions = 0;

        // Shrink the problem size on the left side
        while (compare(*first, *middle))
        {
            ++first;
        }

        auto first2 = middle;
        auto result = cache;
        for (auto first1 = first ; first1 != middle ; ++result)
        {
            if (first2 == last)
            {
                detail::move(first1, middle, result);
                break;
            }
            if (compare(*first2, *first1))
            {
                *result = iter_move(first2);
                ++first2;

                inversions += std::distance(first1, middle);
            }
            else
            {
                *result = iter_move(first1);
                ++first1;
            }
        }

        // Move everything back to the original array, note that
        // everything after first2 hasn't been moved to the cache
        // since it's already in its final place
        detail::move(cache, cache + std::distance(first, first2), first);

        return inversions;
    }

    template<
        typename ResultType,
        typename RandomAccessIterator1,
        typename RandomAccessIterator2,
        typename Compare
    >
    auto count_inversions(RandomAccessIterator1 first, RandomAccessIterator1 last,
                          RandomAccessIterator2 cache, Compare compare)
        -> ResultType
    {
        auto size = std::distance(first, last);
        if (size < 2)
        {
            return 0;
        }

        ResultType inversions = 0;
        auto middle = std::next(first, size / 2);

        inversions += count_inversions<ResultType>(first, middle, cache, compare);
        inversions += count_inversions<ResultType>(middle, last, cache, compare);
        inversions += count_inversions_merge<ResultType>(std::move(first), std::move(middle),
                                                         std::move(last), std::move(cache),
                                                         std::move(compare));
        return inversions;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto count_inversions_quadratic(ForwardIterator first, ForwardIterator last,
                                    Compare compare, Projection projection)
        -> difference_type_t<ForwardIterator>
    {
        using difference_type = difference_type_t<ForwardIterator>;
        auto&& proj = utility::as_function(projection);

        difference_type count = 0;
        for (auto it1 = first ; it1 != last ; ++it1)
        {
            auto&& value = proj(*it1);
            for (auto it2 = std::next(it1) ; it2 != last ; ++it2)
            {
                if (compare(proj(*it2), value))
                {
                    ++count;
                }
            }
        }
        return count;
    }
}}

#endif // CPPSORT_DETAIL_COUNT_INVERSIONS_H_
