/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_BUBBLE_SORT_H_
#define CPPSORT_DETAIL_BUBBLE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/mstd/iterator.h>
#include "swap_if.h"

namespace cppsort::detail
{
    //
    // This sorting algorithm isn't exposed to users of the
    // library, it's only intended to be used as a fallback
    // by other stable algorithms to sort small collections
    //
    // These recursive algorithms tend to compute the size
    // of the collection, so bubble_sort can use it to have
    // a decreasing bound for forward iterators
    //

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto bubble_sort(ForwardIterator first, mstd::iter_difference_t<ForwardIterator> size,
                               Compare compare, Projection projection)
        -> void
    {
        if (size < 2) return;

        while (--size) {
            auto current = first;
            auto next = mstd::next(current);
            for (mstd::iter_difference_t<ForwardIterator> i = 0; i < size; ++i) {
                iter_swap_if(current, next, compare, projection);
                ++next;
                ++current;
            }
        }
    }
}

#endif // CPPSORT_DETAIL_BUBBLE_SORT_H_
