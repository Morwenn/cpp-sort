/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_MOVES_SORT3_H_
#define CPPSORT_DETAIL_LOW_MOVES_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "../rotate_left.h"
#include "../rotate_right.h"
#include "../type_traits.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_moves_sorter_impl<3u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (comp(proj(first[1u]), proj(first[0u]))) {
                if (comp(proj(first[2u]), proj(first[0u]))) {
                    if (comp(proj(first[2u]), proj(first[1u]))) {
                        iter_swap(first, first + 2u);
                    } else {
                        rotate_left<3u>(first);
                    }
                } else {
                    iter_swap(first, first + 1u);
                }
            } else {
                if (comp(proj(first[2u]), proj(first[1u]))) {
                    if (comp(proj(first[2u]), proj(first[0u]))) {
                        rotate_right<3u>(first);
                    } else {
                        iter_swap(first + 1u, first + 2u);
                    }
                }
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_MOVES_SORT3_H_
