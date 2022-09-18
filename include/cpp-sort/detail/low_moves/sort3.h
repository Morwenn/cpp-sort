/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_MOVES_SORT3_H_
#define CPPSORT_DETAIL_LOW_MOVES_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "../rotate_left.h"
#include "../rotate_right.h"

namespace cppsort::detail
{
    template<>
    struct low_moves_sorter_impl<3u>
    {
        template<
            mstd::random_access_iterator Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = mstd::enable_if_t<is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator first, Iterator,
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
}

#endif // CPPSORT_DETAIL_LOW_MOVES_SORT3_H_
