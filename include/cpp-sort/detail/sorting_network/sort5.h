/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../swap_if.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<5u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_
