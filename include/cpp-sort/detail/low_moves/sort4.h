/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_MOVES_SORT4_H_
#define CPPSORT_DETAIL_LOW_MOVES_SORT4_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "../min_element.h"
#include "../type_traits.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_moves_sorter_impl<4u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using utility::iter_swap;

            RandomAccessIterator min = min_element(first, last, compare, projection);
            if (min != first)
            {
                iter_swap(min, first);
            }
            low_moves_sorter<3u>{}(first+1u, last, std::move(compare), std::move(projection));
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_MOVES_SORT4_H_
