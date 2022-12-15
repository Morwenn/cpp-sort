/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_APPLY_PERMUTATION_H_
#define CPPSORT_UTILITY_APPLY_PERMUTATION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/config.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace utility
{
    template<typename RandomAccessIterator1, typename RandomAccessIterator2>
    auto apply_permutation(RandomAccessIterator1 first, RandomAccessIterator1 last,
                           RandomAccessIterator2 indices_first, RandomAccessIterator2 indices_last)
        -> void
    {
        using difference_type = cppsort::detail::difference_type_t<RandomAccessIterator1>;
        using utility::iter_move;
        CPPSORT_ASSERT( (last - first) == (indices_last - indices_first) );
        (void)last;

        auto size = indices_last - indices_first;
        for (difference_type idx = 0; idx < size; ++idx) {
            if (idx != indices_first[idx]) {
                auto current_idx = idx;
                auto tmp = iter_move(first + current_idx);
                do {
                    auto next_idx = indices_first[current_idx];
                    first[current_idx] = iter_move(first + next_idx);
                    indices_first[current_idx] = current_idx;
                    current_idx = next_idx;
                } while (idx != indices_first[current_idx]);
                indices_first[current_idx] = current_idx;
                first[current_idx] = std::move(tmp);
            }
        }
    }

    template<typename RandomAccessIterable1, typename RandomAccessIterable2>
    auto apply_permutation(RandomAccessIterable1&& iterable, RandomAccessIterable2&& indices)
        -> void
    {
        apply_permutation(std::begin(iterable), std::end(iterable),
                          std::begin(indices), std::end(indices));
    }
}}

#endif // CPPSORT_UTILITY_APPLY_PERMUTATION_H_
