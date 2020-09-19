/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_

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
    struct sorting_network_sorter_impl<10u>
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
            iter_swap_if(first + 4u, first + 9u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 2u, first + 7u, compare, projection);
            iter_swap_if(first + 1u, first + 6u, compare, projection);
            iter_swap_if(first, first + 5u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 2u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 7u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_
