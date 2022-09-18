/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_MOVES_SORT2_H_
#define CPPSORT_DETAIL_LOW_MOVES_SORT2_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_traits.h>
#include "../swap_if.h"

namespace cppsort::detail
{
    template<>
    struct low_moves_sorter_impl<2u>
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
            iter_swap_if(first, first + 1u,
                         std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_MOVES_SORT2_H_
