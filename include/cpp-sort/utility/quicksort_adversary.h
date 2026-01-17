/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_QUICKSORT_ADVERSARY_H_
#define CPPSORT_QUICKSORT_ADVERSARY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <numeric>
#include <vector>

namespace cppsort::utility
{
    // Implementation of a quicksort adversary as described by M. D. McIlroy
    // in *A Killer Adversary for Quicksort*

    template<typename Sorter, typename Integer>
    auto quicksort_adversary(Sorter&& sorter, Integer size)
    {
        Integer solid = 0;
        auto gas = size - 1;
        std::vector<Integer> elements(size, gas);

        std::vector<Integer> values(size, 0);
        std::iota(values.begin(), values.end(), 0);

        int pivot_candidate = size; // Too big to match any
        return sorter(values, [&, gas](Integer lhs_idx, Integer rhs_idx) {
            int& lhs = elements[lhs_idx];
            int& rhs = elements[rhs_idx];
            if (lhs == gas && rhs == gas) {
                if (lhs_idx == pivot_candidate) {
                    lhs = solid++;
                } else {
                    rhs = solid++;
                }
            }
            if (lhs == gas) {
                pivot_candidate = lhs_idx;
            } else if (rhs == gas) {
                pivot_candidate = rhs_idx;
            }
            return lhs < rhs;
        });
    }
}

#endif // CPPSORT_QUICKSORT_ADVERSARY_H_