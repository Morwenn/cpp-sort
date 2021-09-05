/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT16_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT16_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<16u>
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
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 8u, first + 12u, compare, projection);
            iter_swap_if(first + 1u, first + 5u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 3u, first + 7u, compare, projection);
            iter_swap_if(first + 11u, first + 15u, compare, projection);
            iter_swap_if(first, first + 8u, compare, projection);
            iter_swap_if(first + 1u, first + 9u, compare, projection);
            iter_swap_if(first + 2u, first + 10u, compare, projection);
            iter_swap_if(first + 3u, first + 11u, compare, projection);
            iter_swap_if(first + 4u, first + 12u, compare, projection);
            iter_swap_if(first + 5u, first + 13u, compare, projection);
            iter_swap_if(first + 6u, first + 14u, compare, projection);
            iter_swap_if(first + 7u, first + 15u, compare, projection);
            iter_swap_if(first + 5u, first + 10u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first + 3u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 7u, first + 13u, compare, projection);
            iter_swap_if(first + 2u, first + 8u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 12u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 60>
        {
            return {{
                {0, 13}, {1, 12}, {2, 15}, {3, 14}, {4, 8}, {5, 6}, {7, 11}, {9, 10},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {6, 13}, {8, 14}, {10, 15}, {11, 12},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 15},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {12, 14}, {13, 15},
                {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {13, 14},
                {1, 4}, {2, 6}, {5, 8}, {7, 10}, {9, 13}, {11, 14},
                {2, 4}, {3, 6}, {9, 12}, {11, 13},
                {3, 5}, {6, 8}, {7, 9}, {10, 12},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12},
                {6, 7}, {8, 9},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT16_H_
