/*
* Copyright (c) 2023-2026 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT41_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT41_H_

namespace cppsort::detail
{
    template<>
    struct sorting_network_sorter_impl<41>
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
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 28, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 10, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 26, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 29, compare, projection);
            iter_swap_if(first + 22, first + 30, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 2, first + 18, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 21, compare, projection);
            iter_swap_if(first + 6, first + 22, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 26, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 12, first + 28, compare, projection);
            iter_swap_if(first + 13, first + 29, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 34, first + 40, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 40, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 17, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 26, compare, projection);
            iter_swap_if(first + 14, first + 28, compare, projection);
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 39, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 2, first + 34, compare, projection);
            iter_swap_if(first + 3, first + 36, compare, projection);
            iter_swap_if(first + 4, first + 35, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 25, compare, projection);
            iter_swap_if(first + 12, first + 32, compare, projection);
            iter_swap_if(first + 15, first + 39, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 2, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 34, compare, projection);
            iter_swap_if(first + 6, first + 33, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 10, first + 32, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 17, first + 35, compare, projection);
            iter_swap_if(first + 19, first + 36, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 37, compare, projection);
            iter_swap_if(first + 27, first + 39, compare, projection);
            iter_swap_if(first + 28, first + 40, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 22, compare, projection);
            iter_swap_if(first + 11, first + 36, compare, projection);
            iter_swap_if(first + 18, first + 33, compare, projection);
            iter_swap_if(first + 20, first + 34, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 28, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 40, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 21, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 19, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 13, first + 34, compare, projection);
            iter_swap_if(first + 14, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 37, compare, projection);
            iter_swap_if(first + 28, first + 35, compare, projection);
            iter_swap_if(first + 29, first + 36, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 31, first + 40, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 20, compare, projection);
            iter_swap_if(first + 13, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 36, compare, projection);
            iter_swap_if(first + 25, first + 34, compare, projection);
            iter_swap_if(first + 26, first + 33, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 28, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 20, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        [[nodiscard]]
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 282>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {32, 34}, {33, 35}, {36, 38}, {37, 39},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {16, 20}, {17, 21}, {18, 22}, {19, 23}, {24, 28}, {25, 29}, {26, 30}, {27, 31}, {32, 36}, {33, 37}, {34, 38}, {35, 39},
                {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}, {16, 24}, {17, 25}, {18, 26}, {19, 27}, {20, 28}, {21, 29}, {22, 30}, {23, 31}, {33, 34}, {36, 40}, {37, 38},
                {0, 16}, {1, 17}, {2, 18}, {3, 19}, {4, 20}, {5, 21}, {6, 22}, {7, 23}, {8, 24}, {9, 25}, {10, 26}, {11, 27}, {12, 28}, {13, 29}, {14, 30}, {15, 31}, {33, 36}, {34, 40},
                {1, 2}, {3, 12}, {4, 8}, {5, 10}, {6, 9}, {7, 11}, {13, 14}, {17, 18}, {19, 28}, {20, 24}, {21, 26}, {22, 25}, {23, 27}, {29, 30}, {32, 33}, {34, 36}, {35, 40},
                {1, 4}, {2, 8}, {3, 20}, {5, 17}, {7, 13}, {9, 10}, {11, 26}, {14, 28}, {16, 32}, {18, 24}, {19, 39}, {21, 22}, {35, 37}, {38, 40},
                {0, 16}, {2, 34}, {3, 36}, {4, 35}, {6, 17}, {11, 25}, {12, 32}, {15, 39}, {19, 21}, {24, 40}, {26, 38},
                {2, 12}, {4, 16}, {5, 34}, {6, 33}, {8, 24}, {10, 32}, {15, 23}, {17, 35}, {19, 36}, {22, 26}, {25, 37}, {27, 39}, {28, 40},
                {1, 6}, {3, 5}, {7, 19}, {8, 12}, {10, 22}, {11, 36}, {18, 33}, {20, 34}, {21, 25}, {28, 37}, {30, 40}, {32, 35},
                {1, 4}, {3, 6}, {8, 16}, {9, 21}, {10, 18}, {11, 19}, {12, 17}, {13, 34}, {14, 33}, {24, 32}, {27, 37}, {28, 35}, {29, 36}, {30, 38}, {31, 40},
                {3, 8}, {6, 10}, {9, 20}, {13, 21}, {14, 22}, {15, 29}, {17, 24}, {23, 36}, {25, 34}, {26, 33}, {27, 30}, {28, 32}, {31, 37}, {39, 40},
                {2, 8}, {5, 9}, {10, 16}, {13, 20}, {14, 18}, {15, 19}, {21, 25}, {22, 26}, {23, 30}, {29, 35}, {33, 34}, {36, 38}, {37, 39},
                {2, 4}, {5, 10}, {7, 13}, {9, 12}, {11, 18}, {14, 17}, {15, 28}, {19, 25}, {20, 24}, {21, 22}, {23, 33}, {26, 32}, {27, 29}, {30, 34}, {31, 36}, {37, 38},
                {3, 4}, {6, 9}, {7, 14}, {11, 20}, {12, 16}, {13, 17}, {15, 21}, {18, 24}, {19, 26}, {22, 28}, {23, 27}, {25, 32}, {29, 33}, {30, 35}, {36, 37},
                {6, 8}, {7, 9}, {10, 12}, {11, 16}, {13, 14}, {15, 20}, {17, 18}, {19, 24}, {21, 22}, {23, 26}, {25, 28}, {27, 30}, {29, 32}, {31, 35},
                {5, 8}, {7, 10}, {9, 12}, {11, 13}, {14, 16}, {15, 17}, {18, 20}, {19, 21}, {22, 24}, {23, 25}, {26, 28}, {27, 29}, {30, 32}, {31, 33}, {34, 35},
                {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36},
            }};
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT41_H_
