/*
 * Copyright (c) 2021-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_
#define CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "iterator_traits.h"
#include "upper_bound.h"

namespace cppsort::detail
{
    // Longest non-decreasing subsequence, computed with an altered
    // patience sorting algorithm - returns a pair containing the
    // size of the LNDS and the size of the collection

    template<
        bool RecomputeSize,
        typename ForwardIterator,
        typename Sentinel,
        typename Compare,
        typename Projection
    >
    auto longest_non_descending_subsequence(ForwardIterator first, Sentinel last,
                                            mstd::iter_difference_t<ForwardIterator> size,
                                            Compare compare, Projection projection)
        -> std::pair<
            mstd::iter_difference_t<ForwardIterator>,
            mstd::iter_difference_t<ForwardIterator>
        >
    {
        constexpr bool constant_time_size =
            mstd::random_access_iterator<ForwardIterator> &&
            mstd::sized_sentinel_for<Sentinel, ForwardIterator>;

        if (first == last) {
            return { 0, 0 };
        }
        if (mstd::next(first) == last) {
            return { 1, 1 };
        }

        // The size is only needed at the end to actually compute Rem. When
        // we do need it, we either:
        // - Retrieve it from parameters if passed explicitly
        // - Call mstd::distance when it is known to run in O(1)
        // - Compute it as we go otherwise, avoiding an extra (n) pass
        if constexpr (RecomputeSize && constant_time_size) {
            size = mstd::distance(first, last);
        }

        auto&& proj = utility::as_function(projection);

        // Top (smaller) elements in patience sorting stacks
        std::vector<ForwardIterator> stack_tops;

        while (first != last) {
            auto it = detail::upper_bound(
                stack_tops.begin(), stack_tops.end(),
                proj(*first), compare, utility::indirect{} | projection
            );

            if (it == stack_tops.end()) {
                // The element is bigger than everything else,
                // create a new "stack" to put it
                stack_tops.emplace_back(first);
            } else {
                // The element is strictly smaller than the top
                // of a given stack, replace the stack top
                *it = first;
            }
            ++first;

            if constexpr (RecomputeSize && not constant_time_size) {
                // Compute the size as-we-go if it can't be retrieved in O(1)
                ++size;
            }
        }

        return { stack_tops.size(), size };
    }
}

#endif // CPPSORT_DETAIL_LONGEST_NON_DESCENDING_SUBSEQUENCE_H_
