/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT30_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT30_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<30u>
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
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 8u, first + 10u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 20u, first + 22u, compare, projection);
            iter_swap_if(first + 24u, first + 26u, compare, projection);
            iter_swap_if(first, first + 4u, compare, projection);
            iter_swap_if(first + 8u, first + 12u, compare, projection);
            iter_swap_if(first + 16u, first + 20u, compare, projection);
            iter_swap_if(first + 24u, first + 28u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 18u, first + 22u, compare, projection);
            iter_swap_if(first, first + 8u, compare, projection);
            iter_swap_if(first + 16u, first + 24u, compare, projection);
            iter_swap_if(first + 2u, first + 10u, compare, projection);
            iter_swap_if(first + 18u, first + 26u, compare, projection);
            iter_swap_if(first + 4u, first + 12u, compare, projection);
            iter_swap_if(first + 20u, first + 28u, compare, projection);
            iter_swap_if(first + 6u, first + 14u, compare, projection);
            iter_swap_if(first, first + 16u, compare, projection);
            iter_swap_if(first + 2u, first + 18u, compare, projection);
            iter_swap_if(first + 4u, first + 20u, compare, projection);
            iter_swap_if(first + 6u, first + 22u, compare, projection);
            iter_swap_if(first + 8u, first + 24u, compare, projection);
            iter_swap_if(first + 10u, first + 26u, compare, projection);
            iter_swap_if(first + 12u, first + 28u, compare, projection);
            iter_swap_if(first + 10u, first + 20u, compare, projection);
            iter_swap_if(first + 12u, first + 18u, compare, projection);
            iter_swap_if(first + 6u, first + 24u, compare, projection);
            iter_swap_if(first + 26u, first + 28u, compare, projection);
            iter_swap_if(first + 14u, first + 22u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 8u, first + 16u, compare, projection);
            iter_swap_if(first + 2u, first + 8u, compare, projection);
            iter_swap_if(first + 14u, first + 26u, compare, projection);
            iter_swap_if(first + 4u, first + 16u, compare, projection);
            iter_swap_if(first + 22u, first + 28u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first + 22u, first + 26u, compare, projection);
            iter_swap_if(first + 6u, first + 16u, compare, projection);
            iter_swap_if(first + 14u, first + 24u, compare, projection);
            iter_swap_if(first + 12u, first + 16u, compare, projection);
            iter_swap_if(first + 20u, first + 24u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 14u, first + 18u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first + 22u, first + 24u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 16u, first + 18u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first + 21u, first + 23u, compare, projection);
            iter_swap_if(first + 25u, first + 27u, compare, projection);
            iter_swap_if(first + 1u, first + 5u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first + 17u, first + 21u, compare, projection);
            iter_swap_if(first + 25u, first + 29u, compare, projection);
            iter_swap_if(first + 3u, first + 7u, compare, projection);
            iter_swap_if(first + 11u, first + 15u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first + 1u, first + 9u, compare, projection);
            iter_swap_if(first + 17u, first + 25u, compare, projection);
            iter_swap_if(first + 3u, first + 11u, compare, projection);
            iter_swap_if(first + 19u, first + 27u, compare, projection);
            iter_swap_if(first + 5u, first + 13u, compare, projection);
            iter_swap_if(first + 21u, first + 29u, compare, projection);
            iter_swap_if(first + 7u, first + 15u, compare, projection);
            iter_swap_if(first + 1u, first + 17u, compare, projection);
            iter_swap_if(first + 3u, first + 19u, compare, projection);
            iter_swap_if(first + 5u, first + 21u, compare, projection);
            iter_swap_if(first + 7u, first + 23u, compare, projection);
            iter_swap_if(first + 9u, first + 25u, compare, projection);
            iter_swap_if(first + 11u, first + 27u, compare, projection);
            iter_swap_if(first + 13u, first + 29u, compare, projection);
            iter_swap_if(first + 11u, first + 21u, compare, projection);
            iter_swap_if(first + 13u, first + 19u, compare, projection);
            iter_swap_if(first + 7u, first + 25u, compare, projection);
            iter_swap_if(first + 27u, first + 29u, compare, projection);
            iter_swap_if(first + 15u, first + 23u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 9u, first + 17u, compare, projection);
            iter_swap_if(first + 3u, first + 9u, compare, projection);
            iter_swap_if(first + 15u, first + 27u, compare, projection);
            iter_swap_if(first + 5u, first + 17u, compare, projection);
            iter_swap_if(first + 23u, first + 29u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 19u, first + 21u, compare, projection);
            iter_swap_if(first + 23u, first + 27u, compare, projection);
            iter_swap_if(first + 7u, first + 17u, compare, projection);
            iter_swap_if(first + 15u, first + 25u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 21u, first + 25u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 15u, first + 19u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 21u, compare, projection);
            iter_swap_if(first + 23u, first + 25u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
            iter_swap_if(first + 20u, first + 21u, compare, projection);
            iter_swap_if(first + 22u, first + 23u, compare, projection);
            iter_swap_if(first + 24u, first + 25u, compare, projection);
            iter_swap_if(first + 26u, first + 27u, compare, projection);
            iter_swap_if(first + 28u, first + 29u, compare, projection);
            iter_swap_if(first + 1u, first + 16u, compare, projection);
            iter_swap_if(first + 3u, first + 18u, compare, projection);
            iter_swap_if(first + 5u, first + 20u, compare, projection);
            iter_swap_if(first + 7u, first + 22u, compare, projection);
            iter_swap_if(first + 9u, first + 24u, compare, projection);
            iter_swap_if(first + 11u, first + 26u, compare, projection);
            iter_swap_if(first + 13u, first + 28u, compare, projection);
            iter_swap_if(first + 1u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 10u, compare, projection);
            iter_swap_if(first + 5u, first + 12u, compare, projection);
            iter_swap_if(first + 7u, first + 14u, compare, projection);
            iter_swap_if(first + 9u, first + 16u, compare, projection);
            iter_swap_if(first + 11u, first + 18u, compare, projection);
            iter_swap_if(first + 13u, first + 20u, compare, projection);
            iter_swap_if(first + 15u, first + 22u, compare, projection);
            iter_swap_if(first + 17u, first + 24u, compare, projection);
            iter_swap_if(first + 19u, first + 26u, compare, projection);
            iter_swap_if(first + 21u, first + 28u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 3u, first + 6u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 9u, first + 12u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 16u, compare, projection);
            iter_swap_if(first + 15u, first + 18u, compare, projection);
            iter_swap_if(first + 17u, first + 20u, compare, projection);
            iter_swap_if(first + 19u, first + 22u, compare, projection);
            iter_swap_if(first + 21u, first + 24u, compare, projection);
            iter_swap_if(first + 23u, first + 26u, compare, projection);
            iter_swap_if(first + 25u, first + 28u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
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
            iter_swap_if(first + 25u, first + 26u, compare, projection);
            iter_swap_if(first + 27u, first + 28u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 172>
        {
            return {{
                {1, 2}, {3, 10}, {4, 14}, {5, 8}, {6, 13}, {7, 12}, {9, 11}, {16, 17}, {18, 25}, {19, 29}, {20, 23}, {21, 28}, {22, 27}, {24, 26},
                {0, 14}, {1, 5}, {2, 8}, {3, 7}, {6, 9}, {10, 12}, {11, 13}, {15, 29}, {16, 20}, {17, 23}, {18, 22}, {21, 24}, {25, 27}, {26, 28},
                {0, 7}, {1, 6}, {2, 9}, {4, 10}, {5, 11}, {8, 13}, {12, 14}, {15, 22}, {16, 21}, {17, 24}, {19, 25}, {20, 26}, {23, 28}, {27, 29},
                {0, 6}, {2, 4}, {3, 5}, {7, 11}, {8, 10}, {9, 12}, {13, 14}, {15, 21}, {17, 19}, {18, 20}, {22, 26}, {23, 25}, {24, 27}, {28, 29},
                {0, 3}, {1, 2}, {4, 7}, {5, 9}, {6, 8}, {10, 11}, {12, 13}, {14, 29}, {15, 18}, {16, 17}, {19, 22}, {20, 24}, {21, 23}, {25, 26}, {27, 28},
                {0, 1}, {2, 3}, {4, 6}, {7, 9}, {10, 12}, {11, 13}, {15, 16}, {17, 18}, {19, 21}, {22, 24}, {25, 27}, {26, 28},
                {0, 15}, {1, 2}, {3, 5}, {8, 10}, {11, 12}, {13, 28}, {16, 17}, {18, 20}, {23, 25}, {26, 27},
                {1, 16}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {12, 27}, {18, 19}, {20, 21}, {22, 23}, {24, 25},
                {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26},
                {2, 17}, {3, 18}, {4, 19}, {5, 6}, {7, 8}, {9, 24}, {10, 25}, {11, 26}, {20, 21}, {22, 23},
                {5, 20}, {6, 21}, {7, 22}, {8, 23}, {9, 16}, {10, 17}, {11, 18}, {12, 19},
                {5, 9}, {6, 10}, {7, 11}, {8, 15}, {13, 20}, {14, 21}, {18, 22}, {19, 23},
                {3, 5}, {4, 8}, {7, 9}, {12, 15}, {13, 16}, {14, 17}, {20, 24}, {21, 25},
                {2, 4}, {6, 8}, {10, 12}, {11, 13}, {14, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27},
                {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT30_H_
