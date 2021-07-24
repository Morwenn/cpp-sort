/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT27_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT27_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<27u>
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
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 10u, first + 14u, compare, projection);
            iter_swap_if(first + 18u, first + 22u, compare, projection);
            iter_swap_if(first, first + 8u, compare, projection);
            iter_swap_if(first + 16u, first + 24u, compare, projection);
            iter_swap_if(first + 2u, first + 10u, compare, projection);
            iter_swap_if(first + 18u, first + 26u, compare, projection);
            iter_swap_if(first + 4u, first + 12u, compare, projection);
            iter_swap_if(first + 6u, first + 14u, compare, projection);
            iter_swap_if(first, first + 16u, compare, projection);
            iter_swap_if(first + 2u, first + 18u, compare, projection);
            iter_swap_if(first + 4u, first + 20u, compare, projection);
            iter_swap_if(first + 6u, first + 22u, compare, projection);
            iter_swap_if(first + 8u, first + 24u, compare, projection);
            iter_swap_if(first + 10u, first + 26u, compare, projection);
            iter_swap_if(first + 10u, first + 20u, compare, projection);
            iter_swap_if(first + 12u, first + 18u, compare, projection);
            iter_swap_if(first + 6u, first + 24u, compare, projection);
            iter_swap_if(first + 14u, first + 22u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 8u, first + 16u, compare, projection);
            iter_swap_if(first + 2u, first + 8u, compare, projection);
            iter_swap_if(first + 14u, first + 26u, compare, projection);
            iter_swap_if(first + 4u, first + 16u, compare, projection);
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
            iter_swap_if(first + 3u, first + 15u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 17u, compare, projection);
            iter_swap_if(first + 1u, first + 25u, compare, projection);
            iter_swap_if(first + 5u, first + 13u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first + 17u, first + 23u, compare, projection);
            iter_swap_if(first + 15u, first + 25u, compare, projection);
            iter_swap_if(first + 11u, first + 19u, compare, projection);
            iter_swap_if(first + 1u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 15u, compare, projection);
            iter_swap_if(first + 21u, first + 23u, compare, projection);
            iter_swap_if(first + 3u, first + 9u, compare, projection);
            iter_swap_if(first + 13u, first + 25u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 23u, first + 25u, compare, projection);
            iter_swap_if(first + 9u, first + 19u, compare, projection);
            iter_swap_if(first + 13u, first + 21u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first + 21u, first + 23u, compare, projection);
            iter_swap_if(first + 3u, first + 15u, compare, projection);
            iter_swap_if(first + 5u, first + 13u, compare, projection);
            iter_swap_if(first + 19u, first + 23u, compare, projection);
            iter_swap_if(first + 3u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 21u, compare, projection);
            iter_swap_if(first + 1u, first + 11u, compare, projection);
            iter_swap_if(first + 5u, first + 11u, compare, projection);
            iter_swap_if(first + 13u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 21u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 11u, compare, projection);
            iter_swap_if(first + 15u, first + 17u, compare, projection);
            iter_swap_if(first + 9u, first + 13u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 17u, first + 19u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
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
            iter_swap_if(first + 1u, first + 16u, compare, projection);
            iter_swap_if(first + 3u, first + 18u, compare, projection);
            iter_swap_if(first + 5u, first + 20u, compare, projection);
            iter_swap_if(first + 7u, first + 22u, compare, projection);
            iter_swap_if(first + 9u, first + 24u, compare, projection);
            iter_swap_if(first + 11u, first + 26u, compare, projection);
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
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 150>
        {
            return {{
                {0, 9}, {1, 6}, {2, 4}, {3, 7}, {5, 8}, {11, 24}, {12, 23}, {13, 26}, {14, 25}, {15, 19}, {16, 17}, {18, 22}, {20, 21},
                {0, 1}, {3, 5}, {4, 10}, {6, 9}, {7, 8}, {11, 16}, {12, 18}, {13, 20}, {14, 15}, {17, 24}, {19, 25}, {21, 26}, {22, 23},
                {1, 3}, {2, 5}, {4, 7}, {8, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 19}, {18, 20}, {21, 22}, {23, 24}, {25, 26},
                {0, 4}, {1, 2}, {3, 7}, {5, 9}, {6, 8}, {11, 13}, {12, 14}, {15, 21}, {16, 22}, {17, 18}, {19, 20}, {23, 25}, {24, 26},
                {0, 1}, {2, 6}, {4, 5}, {7, 8}, {9, 10}, {12, 13}, {14, 23}, {15, 17}, {16, 18}, {19, 21}, {20, 22}, {24, 25},
                {0, 11}, {2, 4}, {3, 6}, {5, 7}, {8, 9}, {12, 15}, {13, 17}, {16, 19}, {18, 21}, {20, 24}, {22, 25},
                {1, 2}, {3, 4}, {5, 6}, {7, 8}, {13, 15}, {14, 17}, {20, 23}, {22, 24},
                {1, 12}, {2, 3}, {4, 5}, {6, 7}, {14, 16}, {17, 19}, {18, 20}, {21, 23},
                {2, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23},
                {3, 14}, {4, 15}, {5, 16}, {10, 21}, {17, 18}, {19, 20},
                {6, 17}, {7, 18}, {8, 19}, {9, 20}, {10, 13}, {14, 22}, {15, 23}, {16, 24},
                {6, 10}, {7, 14}, {8, 11}, {9, 12}, {17, 25}, {18, 26}, {19, 23}, {20, 24},
                {4, 8}, {5, 9}, {11, 15}, {12, 16}, {13, 17}, {18, 22}, {21, 25}, {24, 26},
                {2, 4}, {3, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {25, 26},
                {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT27_H_
