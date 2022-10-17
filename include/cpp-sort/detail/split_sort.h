/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SPLIT_SORT_H_
#define CPPSORT_DETAIL_SPLIT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "inplace_merge.h"

namespace cppsort::detail
{
    template<
        typename BidirectionalIterator,
        typename Compare,
        typename Projection,
        typename Sorter
    >
    auto split_sort(BidirectionalIterator first, BidirectionalIterator last,
                    Compare compare, Projection projection, Sorter&& sorter)
        -> void
    {
        // This algorithm tries to isolate an approximate longest
        // non-decreasing subsequence (LNDS) in the left portion
        // of the collection, and the other elements in the right
        // portion of the collection, then to sort the remaining
        // elements and to merge both portions

        if (first == last || std::next(first) == last) {
            return;
        }

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Read and reorganize elements until middle is found
        auto middle = first; // Last element of the LNDS
        for (auto reader_it = std::next(first); reader_it != last; ++reader_it) {
            if (comp(proj(*reader_it), proj(*middle))) {
                // We remove the top of the subsequence as well as the new element
                if (middle != first) {
                    --middle;
                }
            } else {
                // Everything is fine, add the new element to the subsequence
                ++middle;
                using utility::iter_swap;
                iter_swap(middle, reader_it);
            }
        }

        // Sort the second part of the collection and merge
        std::forward<Sorter>(sorter)(middle, last, compare, projection);
        inplace_merge(first, middle, last, std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_SPLIT_SORT_H_
