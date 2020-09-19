/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ROTATE_RIGHT_H_
#define CPPSORT_DETAIL_ROTATE_RIGHT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<std::size_t N>
    struct rotate_right_n
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator first) const
            -> void
        {
            using utility::iter_move;
            using difference_type = difference_type_t<RandomAccessIterator>;

            auto tmp = iter_move(first + N - 1);
            for (difference_type i = N - 1 ; i > 0 ; --i)
            {
                first[i] = iter_move(first + (i - 1));
            }
            first[0] = std::move(tmp);
        }
    };

    template<>
    struct rotate_right_n<0u>
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator) const
            -> void
        {}
    };

    template<std::size_t N, typename RandomAccessIterator>
    auto rotate_right(RandomAccessIterator first)
        -> void
    {
        return rotate_right_n<N>{}(first);
    }
}}

#endif // CPPSORT_DETAIL_ROTATE_RIGHT_H_
