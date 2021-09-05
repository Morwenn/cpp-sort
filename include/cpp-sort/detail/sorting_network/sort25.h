/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<25u>
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
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 1u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 18u, compare, projection);
            iter_swap_if(first + 4u, first + 17u, compare, projection);
            iter_swap_if(first + 5u, first + 20u, compare, projection);
            iter_swap_if(first + 6u, first + 19u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 15u, first + 22u, compare, projection);
            iter_swap_if(first + 21u, first + 23u, compare, projection);
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first + 1u, first + 15u, compare, projection);
            iter_swap_if(first + 2u, first + 18u, compare, projection);
            iter_swap_if(first + 4u, first + 12u, compare, projection);
            iter_swap_if(first + 5u, first + 21u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 7u, first + 14u, compare, projection);
            iter_swap_if(first + 8u, first + 22u, compare, projection);
            iter_swap_if(first + 9u, first + 16u, compare, projection);
            iter_swap_if(first + 11u, first + 19u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 20u, first + 23u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 1u, first + 7u, compare, projection);
            iter_swap_if(first + 2u, first + 13u, compare, projection);
            iter_swap_if(first + 3u, first + 12u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 8u, first + 14u, compare, projection);
            iter_swap_if(first + 9u, first + 15u, compare, projection);
            iter_swap_if(first + 10u, first + 21u, compare, projection);
            iter_swap_if(first + 11u, first + 20u, compare, projection);
            iter_swap_if(first + 16u, first + 22u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first, first + 5u, compare, projection);
            iter_swap_if(first + 2u, first + 11u, compare, projection);
            iter_swap_if(first + 3u, first + 6u, compare, projection);
            iter_swap_if(first + 4u, first + 10u, compare, projection);
            iter_swap_if(first + 7u, first + 16u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 12u, first + 21u, compare, projection);
            iter_swap_if(first + 13u, first + 19u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 20u, compare, projection);
            iter_swap_if(first + 18u, first + 23u, compare, projection);
            iter_swap_if(first + 2u, first + 7u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first + 8u, first + 11u, compare, projection);
            iter_swap_if(first + 14u, first + 24u, compare, projection);
            iter_swap_if(first + 18u, first + 21u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 21u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first + 22u, first + 24u, compare, projection);
            iter_swap_if(first + 4u, first + 13u, compare, projection);
            iter_swap_if(first + 10u, first + 16u, compare, projection);
            iter_swap_if(first + 11u, first + 15u, compare, projection);
            iter_swap_if(first + 18u, first + 24u, compare, projection);
            iter_swap_if(first + 19u, first + 22u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 8u, first + 11u, compare, projection);
            iter_swap_if(first + 9u, first + 19u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 14u, first + 18u, compare, projection);
            iter_swap_if(first + 16u, first + 20u, compare, projection);
            iter_swap_if(first + 23u, first + 24u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 13u, compare, projection);
            iter_swap_if(first + 9u, first + 14u, compare, projection);
            iter_swap_if(first + 10u, first + 17u, compare, projection);
            iter_swap_if(first + 12u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
            iter_swap_if(first + 20u, first + 21u, compare, projection);
            iter_swap_if(first + 22u, first + 23u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 13u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 12u, first + 18u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 16u, first + 19u, compare, projection);
            iter_swap_if(first + 20u, first + 22u, compare, projection);
            iter_swap_if(first + 21u, first + 23u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 20u, compare, projection);
            iter_swap_if(first + 21u, first + 22u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 13u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 19u, first + 20u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 12u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 20u, first + 21u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 14u, first + 17u, compare, projection);
            iter_swap_if(first + 16u, first + 19u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 132>
        {
            return {{
                {0, 2}, {1, 8}, {3, 18}, {4, 17}, {5, 20}, {6, 19}, {7, 9}, {10, 11}, {12, 13}, {14, 16}, {15, 22}, {21, 23},
                {0, 3}, {1, 15}, {2, 18}, {4, 12}, {5, 21}, {6, 10}, {7, 14}, {8, 22}, {9, 16}, {11, 19}, {13, 17}, {20, 23},
                {0, 4}, {1, 7}, {2, 13}, {3, 12}, {5, 6}, {8, 14}, {9, 15}, {10, 21}, {11, 20}, {16, 22}, {17, 18}, {19, 23},
                {0, 5}, {2, 11}, {3, 6}, {4, 10}, {7, 16}, {8, 9}, {12, 21}, {13, 19}, {14, 15}, {17, 20}, {18, 23},
                {2, 7}, {6, 9}, {8, 11}, {14, 24}, {18, 21},
                {3, 8}, {7, 10}, {11, 12}, {13, 14}, {15, 21}, {18, 20}, {22, 24},
                {4, 13}, {10, 16}, {11, 15}, {18, 24}, {19, 22},
                {1, 4}, {8, 11}, {9, 19}, {13, 17}, {14, 18}, {16, 20}, {23, 24},
                {0, 1}, {4, 5}, {6, 13}, {9, 14}, {10, 17}, {12, 16}, {18, 19}, {20, 21}, {22, 23},
                {2, 6}, {3, 4}, {5, 13}, {7, 9}, {12, 18}, {15, 17}, {16, 19}, {20, 22}, {21, 23},
                {1, 2}, {5, 8}, {6, 7}, {9, 10}, {11, 13}, {14, 15}, {17, 20}, {21, 22},
                {1, 3}, {2, 4}, {5, 6}, {7, 11}, {8, 9}, {10, 13}, {12, 14}, {15, 16}, {17, 18}, {19, 20},
                {2, 3}, {4, 8}, {6, 7}, {9, 12}, {10, 11}, {13, 14}, {15, 17}, {16, 18}, {20, 21},
                {3, 5}, {4, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 15}, {14, 17}, {16, 19},
                {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_
