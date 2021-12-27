/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
   Copyright (c) Marek Kurdej 2014.
   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
    For more information, see http://www.boost.org
*/
#ifndef CPPSORT_DETAIL_LONGEST_ASCENDING_SUBSEQUENCE_H_
#define CPPSORT_DETAIL_LONGEST_ASCENDING_SUBSEQUENCE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include "config.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto las_do_search(RandomAccessIterator first, RandomAccessIterator last,
                       /*output*/ std::vector<difference_type_t<RandomAccessIterator>>& predecessor,
                       /*output*/ std::vector<difference_type_t<RandomAccessIterator>>& lis_tail,
                       Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        difference_type const n = last - first;
        // Length of the longest (increasing) subsequence found so far
        difference_type lis_length = 0;
        // predecessor[k, 0 <= k < n] - stores the index of the predecessor of
        // X[k] in the longest increasing subsequence ending at X[k]
        predecessor.resize(n);
        // lis_tail[j, 0 <= k <= n] - stores the index k of the smallest value
        // X[k] such that there is an increasing subsequence of length j ending
        // at X[k] on the range k <= i (note we have j <= k <= i here, because
        // j represents the length of the increasing subsequence, and k
        // represents the index of its termination.
        // Obviously, we can never have an increasing subsequence of length 13
        // ending at index 11. k <= i by definition).
        lis_tail.resize(n + 1);

        for (difference_type i = 0; i < n; ++i) {
            // Binary search for the largest positive j <= lis_length, such that
            // X[lis_tail[j]] < X[i].
            // After searching, lo is 1 greater than the length of the longest
            // prefix of X[i].
            difference_type new_lis_length;
            {
                difference_type lo = 1;
                difference_type hi = lis_length;
                while (lo <= hi) {
                    difference_type mid = (lo + hi) / 2; // TODO: overflow
                    CPPSORT_ASSERT(mid <= i);
                    if (comp(proj(first[lis_tail[mid]]), proj(first[i]))) {  // X[lis_tail[mid]] < X[i]
                        lo = mid + 1;
                    } else {
                        hi = mid - 1;
                    }
                }
                new_lis_length = lo;
            }

            // The predecessor of X[i] is the last index of the subsequence of
            // length new_lis_length-1
            CPPSORT_ASSERT(new_lis_length > 0);
            predecessor[i] = lis_tail[new_lis_length - 1];

            CPPSORT_ASSERT(new_lis_length <= n);
            CPPSORT_ASSERT(lis_tail[new_lis_length] < n);
            if (new_lis_length > lis_length) {
                // If we found a subsequence longer than any we have found yet,
                // update lis_tail and lis_length
                lis_tail[new_lis_length] = i;
                lis_length = new_lis_length;
            } else if (comp(proj(first[i]), proj(first[lis_tail[new_lis_length]]))) {
                // X[i] < X[lis_tail[new_lis_length]]
                // If we found a smaller last value for the subsequence of
                // length new_lis_length, only update lis_tail
                lis_tail[new_lis_length] = i;
            }
        }
        return lis_length;
    }

    template<typename RandomAccessIterator>
    auto las_do_output(RandomAccessIterator first, RandomAccessIterator /*last*/,
                       difference_type_t<RandomAccessIterator> lis_length,
                       const std::vector<difference_type_t<RandomAccessIterator>>& predecessor,
                       const std::vector<difference_type_t<RandomAccessIterator>>& lis_tail)
        -> std::vector<RandomAccessIterator>
    {
        // Reconstruct the longest ascending subsequence iterators
        std::vector<RandomAccessIterator> lis(lis_length);
        auto k = lis_tail[lis_length];
        for (auto i = lis_length; i--;) {
            lis[i] = first + k;  // &X[k];
            k = predecessor[k];
        }
        return lis;
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto longest_ascending_subsequence(RandomAccessIterator first, RandomAccessIterator last,
                                       Compare compare, Projection projection)
        -> std::vector<RandomAccessIterator>
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        std::vector<difference_type> predecessor;
        std::vector<difference_type> lis_tail;
        auto lis_length = las_do_search(first, last, predecessor, lis_tail,
                                        std::move(compare), std::move(projection));
        return las_do_output(first, last, lis_length, predecessor, lis_tail);
    }
}}

#endif // CPPSORT_DETAIL_LONGEST_ASCENDING_SUBSEQUENCE_H_
