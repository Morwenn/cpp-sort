/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
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
#ifndef CPPSORT_DETAIL_QUICK_MERGE_SORT_H_
#define CPPSORT_DETAIL_QUICK_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "insertion_sort.h"
#include "nth_element.h"
#include "swap_ranges.h"

namespace cppsort
{
namespace detail
{
    constexpr int qmsort_insertion_limit = 32;

    template<typename InputIterator1, typename InputIterator2, typename OutputIterator,
             typename Size, typename Compare, typename Projection>
    auto internal_half_inplace_merge(InputIterator1 first1, InputIterator1 last1,
                                     InputIterator2 first2, InputIterator2 last2,
                                     OutputIterator result, Size min_len,
                                     Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (; min_len != 0 ; --min_len) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);
            if (comp(proj(*first2), proj(*first1))) {
                iter_swap(result, first2);
                ++first2;
            } else {
                iter_swap(result, first1);
                ++first1;
            }
            ++result;
        }

        for (; first1 != last1; ++result) {
            if (first2 == last2) {
                detail::swap_ranges(first1, last1, result);
                return;
            }

            if (comp(proj(*first2), proj(*first1))) {
                iter_swap(result, first2);
                ++first2;
            } else {
                iter_swap(result, first1);
                ++first1;
            }
        }
        // first2 through last2 are already in the right place
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto internal_buffered_inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                                         BidirectionalIterator last, BidirectionalIterator buffer,
                                         Compare compare, Projection projection)
        -> void
    {
        auto buffer_end = detail::swap_ranges(first, middle, buffer);
        internal_half_inplace_merge(buffer, buffer_end, middle, last, first,
                                    std::distance(first, middle),
                                    std::move(compare), std::move(projection));
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto internal_mergesort(BidirectionalIterator first, BidirectionalIterator last,
                            BidirectionalIterator buffer,
                            Compare compare, Projection projection)
        -> void
    {
        if (std::distance(first, last) <= qmsort_insertion_limit) {
            insertion_sort(first, last, std::move(compare), std::move(projection));
            return;
        }

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        auto middle = first + (last - first) / 2; // Ensures left partition is smaller
        internal_mergesort(first, middle, buffer, compare, projection);
        internal_mergesort(middle, last, buffer, compare, projection);

        // Reduce left partition even more if possible
        auto&& mid_value = proj(*middle);
        while (first != middle && not comp(mid_value, proj(*first))) {
            ++first;
        }
        if (first == middle) return;

        internal_buffered_inplace_merge(first, middle, last, buffer,
                                        std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto quick_merge_sort(RandomAccessIterator first, RandomAccessIterator last,
                          Compare compare, Projection projection)
        -> void
    {
        auto size = std::distance(first, last);
        while (size > qmsort_insertion_limit) {
            auto pivot = first + 2 * (size / 3) - 2;
            detail::nth_element(first, pivot, last, compare, projection);
            internal_mergesort(first, pivot, pivot, compare, projection);

            first = pivot;
            size = std::distance(first, last);
        }
        insertion_sort(first, last, std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_QUICK_MERGE_SORT_H_
