/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT31_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT31_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<31u>
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
            sorting_network_sorter<16u>{}(first, first+16u, compare, projection);
            sorting_network_sorter<15u>{}(first+16u, first+31u, compare, projection);

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
            iter_swap_if(first + 14u, first + 30u, compare, projection);
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
            iter_swap_if(first + 13u, first + 29u, compare, projection);
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
            iter_swap_if(first + 27u, first + 29u, compare, projection);
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
            iter_swap_if(first + 29u, first + 30u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 180>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {16, 20}, {17, 21}, {18, 22}, {19, 23}, {24, 28}, {25, 29}, {26, 30},
                {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}, {16, 24}, {17, 25}, {18, 26}, {19, 27}, {20, 28}, {21, 29}, {22, 30},
                {0, 16}, {1, 8}, {2, 4}, {3, 12}, {5, 10}, {6, 9}, {7, 14}, {11, 13}, {17, 24}, {18, 20}, {19, 28}, {21, 26}, {22, 25}, {23, 30}, {27, 29},
                {1, 2}, {3, 5}, {4, 8}, {6, 22}, {7, 11}, {9, 25}, {10, 12}, {13, 14}, {17, 18}, {19, 21}, {20, 24}, {23, 27}, {26, 28}, {29, 30},
                {1, 17}, {2, 18}, {3, 19}, {4, 20}, {5, 10}, {7, 23}, {8, 24}, {11, 27}, {12, 28}, {13, 29}, {14, 30}, {21, 26},
                {3, 17}, {4, 16}, {5, 21}, {6, 18}, {7, 9}, {8, 20}, {10, 26}, {11, 23}, {13, 25}, {14, 28}, {15, 27}, {22, 24},
                {1, 4}, {3, 8}, {5, 16}, {7, 17}, {9, 21}, {10, 22}, {11, 19}, {12, 20}, {14, 24}, {15, 26}, {23, 28}, {27, 30},
                {2, 5}, {7, 8}, {9, 18}, {11, 17}, {12, 16}, {13, 22}, {14, 20}, {15, 19}, {23, 24}, {26, 29},
                {2, 4}, {6, 12}, {9, 16}, {10, 11}, {13, 17}, {14, 18}, {15, 22}, {19, 25}, {20, 21}, {27, 29},
                {5, 6}, {8, 12}, {9, 10}, {11, 13}, {14, 16}, {15, 17}, {18, 20}, {19, 23}, {21, 22}, {25, 26},
                {3, 5}, {6, 7}, {8, 9}, {10, 12}, {11, 14}, {13, 16}, {15, 18}, {17, 20}, {19, 21}, {22, 23}, {24, 25}, {26, 28},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT31_H_
