/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT20_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT20_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<20u>
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
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first + 1u, first + 7u, compare, projection);
            iter_swap_if(first + 2u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 13u, compare, projection);
            iter_swap_if(first + 11u, first + 15u, compare, projection);
            iter_swap_if(first + 12u, first + 18u, compare, projection);
            iter_swap_if(first + 14u, first + 17u, compare, projection);
            iter_swap_if(first + 16u, first + 19u, compare, projection);
            iter_swap_if(first, first + 14u, compare, projection);
            iter_swap_if(first + 1u, first + 11u, compare, projection);
            iter_swap_if(first + 2u, first + 16u, compare, projection);
            iter_swap_if(first + 3u, first + 17u, compare, projection);
            iter_swap_if(first + 4u, first + 12u, compare, projection);
            iter_swap_if(first + 5u, first + 19u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 7u, first + 15u, compare, projection);
            iter_swap_if(first + 8u, first + 18u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 11u, first + 16u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 19u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 1u, first + 6u, compare, projection);
            iter_swap_if(first + 2u, first + 12u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 11u, compare, projection);
            iter_swap_if(first + 7u, first + 17u, compare, projection);
            iter_swap_if(first + 8u, first + 15u, compare, projection);
            iter_swap_if(first + 13u, first + 18u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
            iter_swap_if(first + 1u, first + 6u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 8u, first + 12u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 18u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 15u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 12u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT20_H_
