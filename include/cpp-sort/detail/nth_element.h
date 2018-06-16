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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//            Modified in 2018 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_NTH_ELEMENT_H_
#define CPPSORT_DETAIL_NTH_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "bubble_sort.h"
#include "config.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "partition.h"
#include "selection_sort.h"
#include "swap_if.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto small_sort(ForwardIterator first, ForwardIterator,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection,
                    std::forward_iterator_tag)
        -> void
    {
        // TODO: find something better than bubble sort
        bubble_sort(std::move(first), size,
                    std::move(compare), std::move(projection));
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto small_sort(BidirectionalIterator first, BidirectionalIterator last,
                    difference_type_t<BidirectionalIterator>,
                    Compare compare, Projection projection,
                    std::bidirectional_iterator_tag)
        -> void
    {
        insertion_sort(std::move(first), std::move(last),
                       std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto small_sort(ForwardIterator first, ForwardIterator last,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        small_sort(first, last, size, std::move(compare), std::move(projection),
                   category{});
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto sort3(ForwardIterator x, ForwardIterator y, ForwardIterator z,
               Compare compare, Projection projection)
        -> unsigned
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        unsigned r = 0;
        if (not comp(proj(*y), proj(*x)))          // if x <= y
        {
            if (not comp(proj(*z), proj(*y)))      // if y <= z
                return r;                   // x <= y && y <= z
                                            // x <= y && y > z
            iter_swap(y, z);                // x <= z && y < z
            r = 1;
            if (comp(proj(*y), proj(*x)))   // if x > y
            {
                iter_swap(x, y);            // x < y && y <= z
                r = 2;
            }
            return r;                       // x <= y && y < z
        }
        if (comp(proj(*z), proj(*y)))       // x > y, if y > z
        {
            iter_swap(x, z);                // x < y && y < z
            r = 1;
            return r;
        }
        iter_swap(x, y);                    // x > y && y <= z
        r = 1;                              // x < y && x <= z
        if (comp(proj(*z), proj(*y)))       // if y > z
        {
            iter_swap(y, z);                // x <= y && y < z
            r = 2;
        }
        return r;
    } // x <= y && y <= z

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto iter_median_5(ForwardIterator it1, ForwardIterator it2, ForwardIterator it3,
                       ForwardIterator it4, ForwardIterator it5,
                       Compare compare, Projection projection)
        -> ForwardIterator
    {
        // Median of 5, adapted from https://stackoverflow.com/a/481398/1364752

        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        iter_swap_if(it1, it2, compare, projection);
        iter_swap_if(it3, it4, compare, projection);

        if (comp(proj(*it1), proj(*it3))) {
            iter_swap(it1, it5);
            iter_swap_if(it1, it2, compare, projection);
        } else {
            iter_swap(it3, it5);
            iter_swap_if(it3, it4, compare, projection);
        }

        if (comp(proj(*it1), proj(*it3))) {
            if (comp(proj(*it2), proj(*it3))) {
                return it3;
            }
            return it2;
        } else {
            if (comp(proj(*it4), proj(*it1))) {
                return it1;
            }
            return it4;
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto iter_median_rest(ForwardIterator first, difference_type_t<ForwardIterator> size,
                          Compare compare, Projection projection)
        -> ForwardIterator
    {
        using utility::iter_swap;

        switch (size) {
            case 0:
            case 1:
            case 2:
                return first;
            case 3: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                iter_swap_if(it1, it2, compare, projection);
                iter_swap_if(it2, it3, compare, projection);
                iter_swap_if(it1, it2, compare, projection);
                return it2;
            }
            case 4: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                auto it4 = ++first;
                iter_swap_if(it1, it2, compare, projection);
                iter_swap_if(it3, it4, compare, projection);
                iter_swap_if(it1, it3, compare, projection);
                iter_swap_if(it2, it4, compare, projection);
                iter_swap_if(it2, it3, compare, projection);
                return it2;
            }
            case 5: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                auto it4 = ++first;
                auto it5 = ++first;
                return iter_median_5(it1, it2, it3, it4, it5,
                                     std::move(compare), std::move(projection));
            }
            default:
                CPPSORT_UNREACHABLE;
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto introselect(ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size, int bad_allowed,
                     Compare compare, Projection projection)
        -> ForwardIterator;

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto median_of_medians(ForwardIterator first, ForwardIterator last,
                           difference_type_t<ForwardIterator> size,
                           Compare compare, Projection projection)
        -> ForwardIterator
    {
        using utility::iter_swap;

        if (size <= 5) {
            return iter_median_rest(first, size, std::move(compare), std::move(projection));
        }

        // Iterator over the collection
        auto it = first;
        // Points to the next value to replace by a median-of-5
        auto medians_it = first;

        // We handle first the biggest part that can be rounded to a power
        // of 5, then we handle the rest
        auto rounded_size = (size / 5) * 5;

        // Handle elements 5 by 5
        for (difference_type_t<ForwardIterator> i = 0 ; i < rounded_size / 5 ; ++i) {
            auto it1 = it;
            auto it2 = ++it;
            auto it3 = ++it;
            auto it4 = ++it;
            auto it5 = ++it;

            auto median = iter_median_5(it1, it2, it3, it4, it5, compare, projection);
            iter_swap(medians_it, median);
            ++medians_it;
            ++it;
        }

        // Handle remaining elements
        if (rounded_size != size) {
            auto last_median = iter_median_rest(it, size - rounded_size, compare, projection);
            iter_swap(last_median, medians_it);
            ++medians_it;
        }

        // Rest variables for the next iteration
        last = medians_it;
        size = rounded_size == size ? size / 5 : size / 5 + 1;

        // Mutual recursion with introselect
        return introselect(first, last, size / 2, size, detail::log2(size),
                           std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Get iterator to last element

    template<typename Iterator>
    auto last_it(Iterator first, Iterator,
                 difference_type_t<Iterator> size,
                 std::forward_iterator_tag)
        -> Iterator
    {
        return std::next(first, size - 1);
    }

    template<typename Iterator>
    auto last_it(Iterator, Iterator last,
                 difference_type_t<Iterator>,
                 std::bidirectional_iterator_tag)
        -> Iterator
    {
        return std::prev(last);
    }

    template<typename Iterator>
    auto last_it(Iterator first, Iterator last, difference_type_t<Iterator> size)
        -> Iterator
    {
        using category = iterator_category_t<Iterator>;
        return last_it(first, last, size, category{});
    }

    ////////////////////////////////////////////////////////////
    // Forward nth_element based on introselect

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto introselect(ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size, int bad_allowed,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (size <= 32) {
            small_sort(first, last, size, std::move(compare), std::move(projection));
            return std::next(first, nth_pos);
        }

        // Choose pivot as either median of 9 or median of medians
        auto temp = [&] {
            if (bad_allowed > 0) {
                auto it1 = std::next(first, size / 8);
                auto it2 = std::next(it1, size / 8);
                auto it3 = std::next(it2, size / 8);
                auto middle = std::next(it3, size/2 - 3*(size/8));
                auto it4 = std::next(middle, size / 8);
                auto it5 = std::next(it4, size / 8);
                auto it6 = std::next(it5, size / 8);
                auto last_1 = last_it(it6, last, size - size/2 - 3*(size/8) - 1);

                iter_sort3(first, it1, it2, compare, projection);
                iter_sort3(it3, middle, it4, compare, projection);
                iter_sort3(it5, it6, last_1, compare, projection);
                auto median_it = iter_sort3(it1, middle, it4, std::move(compare), std::move(projection));
                return std::make_pair(median_it, last_1);
            } else {
                auto last_1 = last_it(first, last, size);
                auto median_it = median_of_medians(first, last, size, compare, projection);
                return std::make_pair(median_it, last_1);
            }
        }();

        // Put the pivot at position std::prev(last) and partition
        iter_swap(temp.first, temp.second);
        auto&& pivot1 = proj(*temp.second);
        auto middle1 = detail::partition(
            first, temp.second,
            [&](const auto& elem) { return comp(proj(elem), pivot1); }
        );

        // Put the pivot in its final position and partition
        iter_swap(middle1, temp.second);
        auto&& pivot2 = proj(*middle1);
        auto middle2 = detail::partition(
            std::next(middle1), last,
            [&](const auto& elem) { return not comp(pivot2, proj(elem)); }
        );

        // Recursive call: heuristic trick here: in real world cases,
        // the middle partition is more likely to be smaller than the
        // right one, so computing its size should generally be cheaper
        auto size_left = std::distance(first, middle1);
        auto size_middle = std::distance(middle1, middle2);
        auto size_right = size - size_left - size_middle;

        // TODO: unroll tail recursion
        // We're done if the nth element is in the middle partition
        if (nth_pos < size_left) {
            return introselect(first, middle1, nth_pos,
                               size_left, --bad_allowed,
                               std::move(compare), std::move(projection));
        } else if (nth_pos > size_left + size_middle) {
            return introselect(middle2, last, nth_pos - size_left - size_middle,
                               size_right, --bad_allowed,
                               std::move(compare), std::move(projection));
        }
        // Return an iterator to the nth element
        return std::next(middle1, nth_pos - size_left);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto nth_element(ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size,
                     Compare compare, Projection projection,
                     std::forward_iterator_tag)
        -> ForwardIterator
    {
        return introselect(first, last, nth_pos, size, detail::log2(size),
                           std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Random-access nth_element from libc++

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto nth_element(RandomAccessIterator first, RandomAccessIterator last,
                     difference_type_t<RandomAccessIterator> nth_pos,
                     difference_type_t<RandomAccessIterator>, // unused
                     Compare compare, Projection projection,
                     std::random_access_iterator_tag)
        -> RandomAccessIterator
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        using difference_type = difference_type_t<RandomAccessIterator>;
        constexpr difference_type limit = 7;

        auto nth = first + nth_pos;

        while (true)
        {
        restart:
            if (nth == last)
                return nth;
            difference_type len = last - first;
            switch (len)
            {
                case 0:
                case 1:
                    return nth;
                case 2:
                    if (comp(proj(*--last), proj(*first))) {
                        iter_swap(first, last);
                    }
                    return nth;
                case 3:
                    RandomAccessIterator m = first;
                    sort3(first, ++m, --last, std::move(compare), std::move(projection));
                    return nth;
            }
            if (len <= limit) {
                selection_sort(first, last, std::move(compare), std::move(projection));
                return nth;
            }
            // len > limit >= 3
            RandomAccessIterator m = first + len / 2;
            RandomAccessIterator lm1 = last;
            unsigned n_swaps = sort3(first, m, --lm1, compare, projection);
            // *m is median
            // partition [first, m) < *m and *m <= [m, last)
            // (this inhibits tossing elements equivalent to m around unnecessarily)
            RandomAccessIterator i = first;
            RandomAccessIterator j = lm1;
            // j points beyond range to be tested, *lm1 is known to be <= *m
            // The search going up is known to be guarded but the search coming down isn't.
            // Prime the downward search with a guard.
            if (not comp(proj(*i), proj(*m)))  // if *first == *m
            {
                // *first == *m, *first doesn't go in first part
                // manually guard downward moving j against i
                while (true)
                {
                    if (i == --j)
                    {
                        // *first == *m, *m <= all other elements
                        // Parition instead into [first, i) == *first and *first < [i, last)
                        ++i;  // first + 1
                        j = last;
                        if (not comp(proj(*first), proj(*--j)))  // we need a guard if *first == *(last-1)
                        {
                            while (true)
                            {
                                if (i == j)
                                    return nth;  // [first, last) all equivalent elements
                                if (comp(proj(*first), proj(*i)))
                                {
                                    iter_swap(i, j);
                                    ++n_swaps;
                                    ++i;
                                    break;
                                }
                                ++i;
                            }
                        }
                        // [first, i) == *first and *first < [j, last) and j == last - 1
                        if (i == j)
                            return nth;
                        while (true)
                        {
                            while (not comp(proj(*first), proj(*i)))
                                ++i;
                            while (comp(proj(*first), proj(*--j)))
                                ;
                            if (i >= j)
                                break;
                            iter_swap(i, j);
                            ++n_swaps;
                            ++i;
                        }
                        // [first, i) == *first and *first < [i, last)
                        // The first part is sorted,
                        if (nth < i)
                            return nth;
                        // nth_element the second part
                        // nth_element(i, nth, last, comp);
                        first = i;
                        goto restart;
                    }
                    if (comp(proj(*j), proj(*m)))
                    {
                        iter_swap(i, j);
                        ++n_swaps;
                        break;  // found guard for downward moving j, now use unguarded partition
                    }
                }
            }
            ++i;
            // j points beyond range to be tested, *lm1 is known to be <= *m
            // if not yet partitioned...
            if (i < j)
            {
                // known that *(i - 1) < *m
                while (true)
                {
                    // m still guards upward moving i
                    while (comp(proj(*i), proj(*m)))
                        ++i;
                    // It is now known that a guard exists for downward moving j
                    while (not comp(proj(*--j), proj(*m)))
                        ;
                    if (i >= j)
                        break;
                    iter_swap(i, j);
                    ++n_swaps;
                    // It is known that m != j
                    // If m just moved, follow it
                    if (m == i)
                        m = j;
                    ++i;
                }
            }
            // [first, i) < *m and *m <= [i, last)
            if (i != m && comp(proj(*m), proj(*i)))
            {
                iter_swap(i, m);
                ++n_swaps;
            }
            // [first, i) < *i and *i <= [i+1, last)
            if (nth == i)
                return nth;
            if (n_swaps == 0)
            {
                // We were given a perfectly partitioned sequence.  Coincidence?
                if (nth < i)
                {
                    // Check for [first, i) already sorted
                    j = m = first;
                    while (++j != i)
                    {
                        if (comp(proj(*j), proj(*m)))
                            // not yet sorted, so sort
                            goto not_sorted;
                        m = j;
                    }
                    // [first, i) sorted
                    return nth;
                }
                else
                {
                    // Check for [i, last) already sorted
                    j = m = i;
                    while (++j != last)
                    {
                        if (comp(proj(*j), proj(*m)))
                            // not yet sorted, so sort
                            goto not_sorted;
                        m = j;
                    }
                    // [i, last) sorted
                    return nth;
                }
            }
    not_sorted:
            // nth_element on range containing nth
            if (nth < i)
            {
                // nth_element(first, nth, i, comp);
                last = i;
            }
            else
            {
                // nth_element(i+1, nth, last, comp);
                first = ++i;
            }
        }
    }

    ////////////////////////////////////////////////////////////
    // Generic nth_element overload, slightly modified compared
    // to the standard library one to avoid recomputing sizes
    // over and over again, which might be too expensive for
    // forward and bidirectional iterators

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto nth_element(ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        using category = iterator_category_t<ForwardIterator>;
        return detail::nth_element(first, last, nth_pos, size,
                                   std::move(compare), std::move(projection),
                                   category{});
    }
}}

#endif // CPPSORT_DETAIL_NTH_ELEMENT_H_
