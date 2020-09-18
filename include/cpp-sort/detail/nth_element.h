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
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
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
#include "introselect.h"
#include "iterator_traits.h"
#include "selection_sort.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // nth_element for forward iterators with introselect

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
    // nth_element for random-access iterators from libc++

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
