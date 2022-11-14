/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_APPLY_PERMUTATION_H_
#define CPPSORT_UTILITY_APPLY_PERMUTATION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include "../detail/config.h"

namespace cppsort
{
namespace utility
{
    template<
        mstd::random_access_iterator Iterator1,
        mstd::sentinel_for<Iterator1> Sentinel1,
        mstd::random_access_iterator Iterator2,
        mstd::sentinel_for<Iterator2> Sentinel2
    >
    auto apply_permutation(Iterator1 first, Sentinel1 last,
                           Iterator2 indices_first, Sentinel2 indices_last)
        -> void
    {
        CPPSORT_ASSERT(mstd::distance(first, last) == mstd::distance(indices_first, indices_last));

        auto size = mstd::distance(indices_first, indices_last);
        for (mstd::iter_difference_t<Iterator1> idx = 0; idx < size; ++idx) {
            if (idx != indices_first[idx]) {
                auto current_idx = idx;
                auto tmp = mstd::iter_move(first + current_idx);
                do {
                    auto next_idx = indices_first[current_idx];
                    first[current_idx] = mstd::iter_move(first + next_idx);
                    indices_first[current_idx] = current_idx;
                    current_idx = next_idx;
                } while (idx != indices_first[current_idx]);
                indices_first[current_idx] = current_idx;
                first[current_idx] = std::move(tmp);
            }
        }
    }

    template<mstd::random_access_range Range1, mstd::random_access_range Range2>
    auto apply_permutation(Range1&& range, Range2&& indices)
        -> void
    {
        apply_permutation(mstd::begin(range), mstd::end(range),
                          mstd::begin(indices), mstd::end(indices));
    }
}}

#endif // CPPSORT_UTILITY_APPLY_PERMUTATION_H_
