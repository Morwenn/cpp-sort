/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<28u>
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
            iter_swap_if(first, first + 9u, compare, projection);
            iter_swap_if(first + 1u, first + 20u, compare, projection);
            iter_swap_if(first + 2u, first + 21u, compare, projection);
            iter_swap_if(first + 3u, first + 22u, compare, projection);
            iter_swap_if(first + 4u, first + 19u, compare, projection);
            iter_swap_if(first + 5u, first + 24u, compare, projection);
            iter_swap_if(first + 6u, first + 25u, compare, projection);
            iter_swap_if(first + 7u, first + 26u, compare, projection);
            iter_swap_if(first + 8u, first + 23u, compare, projection);
            iter_swap_if(first + 10u, first + 15u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 12u, first + 17u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 27u, compare, projection);
            iter_swap_if(first, first + 18u, compare, projection);
            iter_swap_if(first + 1u, first + 7u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 27u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 16u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first + 20u, first + 26u, compare, projection);
            iter_swap_if(first + 21u, first + 25u, compare, projection);
            iter_swap_if(first + 22u, first + 24u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 19u, compare, projection);
            iter_swap_if(first + 6u, first + 20u, compare, projection);
            iter_swap_if(first + 7u, first + 21u, compare, projection);
            iter_swap_if(first + 8u, first + 22u, compare, projection);
            iter_swap_if(first + 9u, first + 18u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 23u, first + 24u, compare, projection);
            iter_swap_if(first + 25u, first + 26u, compare, projection);
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first + 1u, first + 10u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 17u, first + 26u, compare, projection);
            iter_swap_if(first + 19u, first + 22u, compare, projection);
            iter_swap_if(first + 20u, first + 21u, compare, projection);
            iter_swap_if(first + 24u, first + 27u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 7u, compare, projection);
            iter_swap_if(first + 3u, first + 10u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 24u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first + 20u, first + 25u, compare, projection);
            iter_swap_if(first + 26u, first + 27u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 19u, compare, projection);
            iter_swap_if(first + 8u, first + 20u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 21u, first + 25u, compare, projection);
            iter_swap_if(first + 22u, first + 23u, compare, projection);
            iter_swap_if(first + 24u, first + 26u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 12u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 15u, first + 22u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 19u, first + 20u, compare, projection);
            iter_swap_if(first + 23u, first + 25u, compare, projection);
            iter_swap_if(first + 2u, first + 9u, compare, projection);
            iter_swap_if(first + 4u, first + 11u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 13u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 14u, first + 20u, compare, projection);
            iter_swap_if(first + 16u, first + 23u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first + 18u, first + 25u, compare, projection);
            iter_swap_if(first + 21u, first + 22u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 16u, compare, projection);
            iter_swap_if(first + 4u, first + 9u, compare, projection);
            iter_swap_if(first + 6u, first + 12u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 11u, first + 24u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 15u, first + 21u, compare, projection);
            iter_swap_if(first + 18u, first + 23u, compare, projection);
            iter_swap_if(first + 25u, first + 26u, compare, projection);
            iter_swap_if(first + 2u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 7u, compare, projection);
            iter_swap_if(first + 6u, first + 16u, compare, projection);
            iter_swap_if(first + 9u, first + 15u, compare, projection);
            iter_swap_if(first + 11u, first + 21u, compare, projection);
            iter_swap_if(first + 12u, first + 18u, compare, projection);
            iter_swap_if(first + 19u, first + 25u, compare, projection);
            iter_swap_if(first + 20u, first + 23u, compare, projection);
            iter_swap_if(first + 22u, first + 24u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 15u, compare, projection);
            iter_swap_if(first + 12u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first + 19u, first + 22u, compare, projection);
            iter_swap_if(first + 24u, first + 25u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 21u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first + 21u, first + 22u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
            iter_swap_if(first + 20u, first + 21u, compare, projection);
            iter_swap_if(first + 22u, first + 23u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 19u, first + 20u, compare, projection);
            iter_swap_if(first + 21u, first + 22u, compare, projection);
            iter_swap_if(first + 23u, first + 24u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 155>
        {
            return {{
                {0, 9}, {1, 20}, {2, 21}, {3, 22}, {4, 19}, {5, 24}, {6, 25}, {7, 26}, {8, 23}, {10, 15}, {11, 13}, {12, 17}, {14, 16}, {18, 27},
                {0, 18}, {1, 7}, {2, 6}, {3, 5}, {4, 8}, {9, 27}, {10, 12}, {11, 14}, {13, 16}, {15, 17}, {19, 23}, {20, 26}, {21, 25}, {22, 24},
                {1, 2}, {3, 4}, {5, 19}, {6, 20}, {7, 21}, {8, 22}, {9, 18}, {10, 11}, {12, 14}, {13, 15}, {16, 17}, {23, 24}, {25, 26},
                {0, 3}, {1, 10}, {5, 8}, {6, 7}, {11, 13}, {14, 16}, {17, 26}, {19, 22}, {20, 21}, {24, 27},
                {0, 1}, {2, 7}, {3, 10}, {4, 8}, {12, 13}, {14, 15}, {17, 24}, {19, 23}, {20, 25}, {26, 27},
                {1, 3}, {2, 6}, {4, 5}, {7, 19}, {8, 20}, {11, 12}, {13, 14}, {15, 16}, {21, 25}, {22, 23}, {24, 26},
                {2, 4}, {5, 12}, {7, 8}, {9, 11}, {10, 14}, {13, 17}, {15, 22}, {16, 18}, {19, 20}, {23, 25},
                {2, 9}, {4, 11}, {5, 6}, {7, 13}, {8, 10}, {14, 20}, {16, 23}, {17, 19}, {18, 25}, {21, 22},
                {1, 2}, {3, 16}, {4, 9}, {6, 12}, {10, 14}, {11, 24}, {13, 17}, {15, 21}, {18, 23}, {25, 26},
                {2, 8}, {3, 5}, {4, 7}, {6, 16}, {9, 15}, {11, 21}, {12, 18}, {19, 25}, {20, 23}, {22, 24},
                {2, 3}, {5, 8}, {7, 9}, {11, 15}, {12, 16}, {18, 20}, {19, 22}, {24, 25},
                {6, 8}, {10, 12}, {11, 13}, {14, 16}, {15, 17}, {19, 21},
                {5, 6}, {8, 10}, {9, 11}, {12, 13}, {14, 15}, {16, 18}, {17, 19}, {21, 22},
                {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 14}, {13, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_
