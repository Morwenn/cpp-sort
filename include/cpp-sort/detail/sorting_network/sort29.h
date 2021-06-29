/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT29_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT29_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<29u>
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
            sorting_network_sorter<16u>{}(first, first+16u, compare, projection);
            sorting_network_sorter<13u>{}(first+16u, first+29u, compare, projection);

            iter_swap_if(first, first + 16u, compare, projection);
            iter_swap_if(first + 8u, first + 24u, compare, projection);
            iter_swap_if(first + 8u, first + 16u, compare, projection);
            iter_swap_if(first + 4u, first + 20u, compare, projection);
            iter_swap_if(first + 12u, first + 28u, compare, projection);
            iter_swap_if(first + 12u, first + 20u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 12u, first + 16u, compare, projection);
            iter_swap_if(first + 20u, first + 24u, compare, projection);
            iter_swap_if(first + 2u, first + 18u, compare, projection);
            iter_swap_if(first + 10u, first + 26u, compare, projection);
            iter_swap_if(first + 10u, first + 18u, compare, projection);
            iter_swap_if(first + 6u, first + 22u, compare, projection);
            iter_swap_if(first + 14u, first + 22u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 14u, first + 18u, compare, projection);
            iter_swap_if(first + 22u, first + 26u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 10u, first + 12u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first + 22u, first + 24u, compare, projection);
            iter_swap_if(first + 26u, first + 28u, compare, projection);
            iter_swap_if(first + 1u, first + 17u, compare, projection);
            iter_swap_if(first + 9u, first + 25u, compare, projection);
            iter_swap_if(first + 9u, first + 17u, compare, projection);
            iter_swap_if(first + 5u, first + 21u, compare, projection);
            iter_swap_if(first + 13u, first + 21u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 21u, first + 25u, compare, projection);
            iter_swap_if(first + 3u, first + 19u, compare, projection);
            iter_swap_if(first + 11u, first + 27u, compare, projection);
            iter_swap_if(first + 11u, first + 19u, compare, projection);
            iter_swap_if(first + 7u, first + 23u, compare, projection);
            iter_swap_if(first + 15u, first + 23u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 15u, first + 19u, compare, projection);
            iter_swap_if(first + 23u, first + 27u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 21u, compare, projection);
            iter_swap_if(first + 23u, first + 25u, compare, projection);
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
            -> std::array<utility::index_pair<DifferenceType>, 165>
        {
            return {{
                {0, 12}, {1, 10}, {2, 9}, {3, 7}, {5, 11}, {6, 8}, {13, 26}, {14, 25}, {15, 28}, {16, 27}, {17, 21}, {18, 19}, {20, 24}, {22, 23},
                {1, 6}, {2, 3}, {4, 11}, {7, 9}, {8, 10}, {13, 18}, {14, 20}, {15, 22}, {16, 17}, {19, 26}, {21, 27}, {23, 28}, {24, 25},
                {0, 4}, {1, 2}, {3, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 21}, {20, 22}, {23, 24}, {25, 26}, {27, 28},
                {4, 6}, {5, 9}, {8, 11}, {10, 12}, {13, 15}, {14, 16}, {17, 23}, {18, 24}, {19, 20}, {21, 22}, {25, 27}, {26, 28},
                {0, 5}, {3, 8}, {4, 7}, {6, 11}, {9, 10}, {14, 15}, {16, 25}, {17, 19}, {18, 20}, {21, 23}, {22, 24}, {26, 27},
                {0, 1}, {2, 5}, {6, 9}, {7, 8}, {10, 11}, {14, 17}, {15, 19}, {18, 21}, {20, 23}, {22, 26}, {24, 27},
                {0, 13}, {1, 3}, {2, 4}, {5, 6}, {9, 10}, {15, 17}, {16, 19}, {22, 25}, {24, 26},
                {1, 2}, {3, 4}, {5, 7}, {6, 8}, {16, 18}, {19, 21}, {20, 22}, {23, 25},
                {1, 14}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25},
                {2, 15}, {3, 4}, {5, 6}, {10, 23}, {11, 24}, {12, 25}, {19, 20}, {21, 22},
                {3, 16}, {4, 17}, {5, 18}, {6, 19}, {7, 20}, {8, 21}, {9, 22}, {10, 15},
                {6, 10}, {8, 13}, {9, 14}, {11, 16}, {12, 17}, {18, 26}, {19, 27}, {20, 28},
                {4, 8}, {5, 9}, {7, 11}, {12, 13}, {14, 18}, {15, 19}, {16, 20}, {17, 21}, {22, 26}, {23, 27}, {24, 28},
                {2, 4}, {3, 5}, {6, 8}, {7, 9}, {10, 12}, {11, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27},
                {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT29_H_
