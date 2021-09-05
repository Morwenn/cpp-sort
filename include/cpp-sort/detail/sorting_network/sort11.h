/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<11u>
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
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 1u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 1u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 3u, first + 7u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 35>
        {
            return {{
                {0, 9}, {1, 6}, {2, 4}, {3, 7}, {5, 8},
                {0, 1}, {3, 5}, {4, 10}, {6, 9}, {7, 8},
                {1, 3}, {2, 5}, {4, 7}, {8, 10},
                {0, 4}, {1, 2}, {3, 7}, {5, 9}, {6, 8},
                {0, 1}, {2, 6}, {4, 5}, {7, 8}, {9, 10},
                {2, 4}, {3, 6}, {5, 7}, {8, 9},
                {1, 2}, {3, 4}, {5, 6}, {7, 8},
                {2, 3}, {4, 5}, {6, 7},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_
