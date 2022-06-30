/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SWAP_RANGES_H_
#define CPPSORT_DETAIL_SWAP_RANGES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "move.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // swap_ranges_overlap
    //
    // Most basic swap_ranges implementation, somehow tolerates
    // overlapping ranges - at least enough to get some of the
    // library's algorithms to work

    template<typename ForwardIterator1, typename ForwardIterator2>
    auto swap_ranges_overlap(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
        -> ForwardIterator2
    {
        while (first1 != last1) {
            using utility::iter_swap;
            iter_swap(first1, first2);

            ++first1;
            ++first2;
        }
        return first2;
    }

    ////////////////////////////////////////////////////////////
    // swap_ranges_inner
    //
    // swap_ranges flavor to use when two ranges belong to the
    // same collection but can not overlap, might also provide
    // additional diagnostics when the precondition is violated

    template<typename ForwardIterator>
    auto swap_ranges_inner_impl(std::forward_iterator_tag,
                                ForwardIterator first1, ForwardIterator last1,
                                ForwardIterator first2)
        -> ForwardIterator
    {
#ifdef CPPSORT_ENABLE_AUDITS
        bool ranges_overlap = false;
        // This check assumes that first1 <= last1
        auto last2 = std::next(first2, std::distance(first1, last1));
        for (auto it = first1 ; it != last1 ; ++it) {
            if (it == first2) {
                ranges_overlap = true;
            }
            if (it == last2 && it != first1) {
                ranges_overlap = true;
            }
        }
        CPPSORT_AUDIT(not ranges_overlap);
#endif
        return swap_ranges_overlap(first1, last1, first2);
    }

#if defined(_USE_STD_VECTOR_ALGORITHMS) && _USE_STD_VECTOR_ALGORITHMS
    template<typename RandomAccessIterator>
    auto swap_ranges_inner_impl(std::random_access_iterator_tag,
                                RandomAccessIterator first1, RandomAccessIterator last1,
                                RandomAccessIterator first2)
        -> detail::enable_if_t<
            not detail::has_iter_move_v<RandomAccessIterator>,
            RandomAccessIterator
        >
    {
        CPPSORT_ASSERT(first1 <= last1);

        auto last2 = first2 + (last1 - first1);
        (void)last2;
        CPPSORT_ASSERT(not (first2 >= first1 && first2 < last1));
        CPPSORT_ASSERT(not (last2 > first1 && last2 <= last1));

        return std::_Swap_ranges_unchecked(first1, last1, first2);
    }
#endif

    template<typename RandomAccessIterator>
    auto swap_ranges_inner_impl(std::random_access_iterator_tag,
                                RandomAccessIterator first1, RandomAccessIterator last1,
                                RandomAccessIterator first2)
#if defined(_USE_STD_VECTOR_ALGORITHMS) && _USE_STD_VECTOR_ALGORITHMS
        -> detail::enable_if_t<
            detail::has_iter_move_v<RandomAccessIterator>,
            RandomAccessIterator
        >
#else
        -> RandomAccessIterator
#endif
    {
        CPPSORT_ASSERT(first1 <= last1);

        auto last2 = first2 + (last1 - first1);
        (void)last2;
        CPPSORT_ASSERT(not (first2 >= first1 && first2 < last1));
        CPPSORT_ASSERT(not (last2 > first1 && last2 <= last1));

        return detail::swap_ranges_overlap(first1, last1, first2);
    }

    template<typename ForwardIterator>
    auto swap_ranges_inner(ForwardIterator first1, ForwardIterator last1, ForwardIterator first2)
        -> ForwardIterator
    {
        using category = iterator_category_t<ForwardIterator>;
        return swap_ranges_inner_impl(category{}, first1, last1, first2);
    }
}}

#endif // CPPSORT_DETAIL_SWAP_RANGES_H_
