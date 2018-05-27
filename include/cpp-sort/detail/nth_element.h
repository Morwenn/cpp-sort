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
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "selection_sort.h"

namespace cppsort
{
namespace detail
{
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
    auto nth_element(RandomAccessIterator first, RandomAccessIterator nth, RandomAccessIterator last,
                     Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        using difference_type = difference_type_t<RandomAccessIterator>;
        constexpr difference_type limit = 7;

        while (true)
        {
        restart:
            if (nth == last)
                return;
            difference_type len = last - first;
            switch (len)
            {
                case 0:
                case 1:
                    return;
                case 2:
                    if (comp(proj(*--last), proj(*first))) {
                        iter_swap(first, last);
                    }
                    return;
                case 3:
                    RandomAccessIterator m = first;
                    sort3(first, ++m, --last, std::move(compare), std::move(projection));
                    return;
            }
            if (len <= limit) {
                selection_sort(first, last, std::move(compare), std::move(projection));
                return;
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
                                    return;  // [first, last) all equivalent elements
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
                            return;
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
                            return;
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
                return;
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
                    return;
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
                    return;
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
}}

#endif // CPPSORT_DETAIL_NTH_ELEMENT_H_
