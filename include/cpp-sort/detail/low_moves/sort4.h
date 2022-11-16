/*
 * Copyright (c) 2015-2022 Morwenn
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
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/iter_move.h>
#include "../min_element.h"

namespace cppsort::detail
{
    template<>
    struct low_moves_sorter_impl<4u>
    {
        template<
            mstd::random_access_iterator Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires is_projection_iterator_v<Projection, Iterator, Compare>
        auto operator()(Iterator first, Iterator last,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using utility::iter_swap;

            auto min = min_element(first, last, compare, projection);
            if (min != first) {
                iter_swap(min, first);
            }
            low_moves_sorter<3u>{}(first+1u, last, std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_MOVES_SORT4_H_
