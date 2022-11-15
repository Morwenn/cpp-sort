/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ROTATE_LEFT_H_
#define CPPSORT_DETAIL_ROTATE_LEFT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>
#include <cpp-sort/mstd/iterator.h>

namespace cppsort::detail
{
    template<std::size_t N>
    struct rotate_left_n
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator first) const
            -> void
        {
            using difference_type = mstd::iter_difference_t<RandomAccessIterator>;

            auto tmp = mstd::iter_move(first);
            for (difference_type i = 0; i < static_cast<difference_type>(N - 1); ++i) {
                first[i] = mstd::iter_move(first + (i + 1));
            }
            first[N - 1] = std::move(tmp);
        }
    };

    template<>
    struct rotate_left_n<0u>
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator) const
            -> void
        {}
    };

    template<std::size_t N, typename RandomAccessIterator>
    auto rotate_left(RandomAccessIterator first)
        -> void
    {
        return rotate_left_n<N>{}(first);
    }
}

#endif // CPPSORT_DETAIL_ROTATE_LEFT_H_
