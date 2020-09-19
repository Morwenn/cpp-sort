/*
 * Copyright (c) 2016 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SWAP_RANGES_H_
#define CPPSORT_DETAIL_SWAP_RANGES_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/iter_move.h>

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator1, typename ForwardIterator2>
    auto swap_ranges(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2)
        -> ForwardIterator2
    {
        while (first1 != last1)
        {
            using utility::iter_swap;
            iter_swap(first1, first2);

            ++first1;
            ++first2;
        }
        return first2;
    }
}}

#endif // CPPSORT_DETAIL_SWAP_RANGES_H_
