/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SELECTION_SORT_H_
#define CPPSORT_DETAIL_SELECTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/iter_move.h>
#include "min_element.h"

namespace cppsort::detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto selection_sort(ForwardIterator first, ForwardIterator last,
                                  Compare compare, Projection projection)
        -> void
    {
        for (auto it = first; it != last; ++it) {
            using utility::iter_swap;
            iter_swap(it, unchecked_min_element(it, last, compare, projection));
        }
    }
}

#endif // CPPSORT_DETAIL_SELECTION_SORT_H_
