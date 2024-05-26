/*
* Copyright (c) 2023-2024 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT42_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT42_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<42>
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
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 10, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 32, first + 40, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first, first + 26, compare, projection);
            iter_swap_if(first + 1, first + 27, compare, projection);
            iter_swap_if(first + 2, first + 28, compare, projection);
            iter_swap_if(first + 3, first + 29, compare, projection);
            iter_swap_if(first + 4, first + 30, compare, projection);
            iter_swap_if(first + 5, first + 31, compare, projection);
            iter_swap_if(first + 6, first + 32, compare, projection);
            iter_swap_if(first + 7, first + 33, compare, projection);
            iter_swap_if(first + 8, first + 34, compare, projection);
            iter_swap_if(first + 9, first + 35, compare, projection);
            iter_swap_if(first + 10, first + 36, compare, projection);
            iter_swap_if(first + 11, first + 37, compare, projection);
            iter_swap_if(first + 12, first + 38, compare, projection);
            iter_swap_if(first + 13, first + 39, compare, projection);
            iter_swap_if(first + 14, first + 40, compare, projection);
            iter_swap_if(first + 15, first + 41, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 38, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 4, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 15, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 26, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 37, compare, projection);
            iter_swap_if(first + 28, first + 34, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 2, first + 16, compare, projection);
            iter_swap_if(first + 3, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 26, compare, projection);
            iter_swap_if(first + 8, first + 19, compare, projection);
            iter_swap_if(first + 11, first + 23, compare, projection);
            iter_swap_if(first + 12, first + 27, compare, projection);
            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first + 15, first + 36, compare, projection);
            iter_swap_if(first + 18, first + 30, compare, projection);
            iter_swap_if(first + 21, first + 38, compare, projection);
            iter_swap_if(first + 22, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 25, first + 39, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 26, compare, projection);
            iter_swap_if(first + 8, first + 17, compare, projection);
            iter_swap_if(first + 9, first + 30, compare, projection);
            iter_swap_if(first + 10, first + 19, compare, projection);
            iter_swap_if(first + 11, first + 32, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 34, compare, projection);
            iter_swap_if(first + 16, first + 25, compare, projection);
            iter_swap_if(first + 21, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 31, compare, projection);
            iter_swap_if(first + 23, first + 35, compare, projection);
            iter_swap_if(first + 24, first + 33, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 40, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 28, compare, projection);
            iter_swap_if(first + 12, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 30, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 32, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 34, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 24, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 35, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 31, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
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
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 291>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 39}, {40, 41},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 24}, {17, 25}, {18, 22}, {19, 23}, {26, 28}, {27, 29}, {30, 32}, {31, 33}, {34, 36}, {35, 37}, {38, 40}, {39, 41},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {16, 18}, {17, 20}, {19, 22}, {21, 24}, {23, 25}, {26, 30}, {27, 31}, {28, 32}, {29, 33}, {34, 38}, {35, 39}, {36, 40}, {37, 41},
                {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}, {17, 18}, {19, 21}, {20, 22}, {23, 24}, {26, 34}, {27, 35}, {28, 36}, {29, 37}, {30, 38}, {31, 39}, {32, 40}, {33, 41},
                {0, 26}, {1, 27}, {2, 28}, {3, 29}, {4, 30}, {5, 31}, {6, 32}, {7, 33}, {8, 34}, {9, 35}, {10, 36}, {11, 37}, {12, 38}, {13, 39}, {14, 40}, {15, 41}, {16, 17}, {18, 20}, {21, 23}, {24, 25},
                {0, 16}, {1, 2}, {3, 12}, {4, 8}, {5, 10}, {6, 9}, {7, 11}, {13, 14}, {17, 19}, {20, 21}, {22, 24}, {25, 41}, {27, 28}, {29, 38}, {30, 34}, {31, 36}, {32, 35}, {33, 37}, {39, 40},
                {4, 16}, {5, 6}, {7, 13}, {9, 10}, {14, 21}, {15, 24}, {17, 26}, {18, 19}, {20, 27}, {22, 23}, {25, 37}, {28, 34}, {31, 32}, {35, 36},
                {1, 17}, {2, 16}, {3, 20}, {5, 26}, {8, 19}, {11, 23}, {12, 27}, {14, 29}, {15, 36}, {18, 30}, {21, 38}, {22, 33}, {24, 40}, {25, 39},
                {1, 4}, {3, 5}, {6, 18}, {7, 26}, {8, 17}, {9, 30}, {10, 19}, {11, 32}, {14, 20}, {15, 34}, {16, 25}, {21, 27}, {22, 31}, {23, 35}, {24, 33}, {36, 38}, {37, 40},
                {2, 6}, {9, 14}, {11, 28}, {12, 18}, {13, 30}, {16, 17}, {19, 21}, {20, 22}, {23, 29}, {24, 25}, {27, 32}, {35, 39},
                {2, 8}, {5, 9}, {6, 16}, {7, 20}, {10, 17}, {11, 12}, {13, 22}, {15, 18}, {19, 28}, {21, 34}, {23, 26}, {24, 31}, {25, 35}, {29, 30}, {32, 36}, {33, 39},
                {4, 8}, {7, 11}, {10, 12}, {13, 14}, {17, 18}, {23, 24}, {27, 28}, {29, 31}, {30, 34}, {33, 37},
                {2, 4}, {6, 8}, {9, 10}, {11, 16}, {12, 20}, {15, 17}, {18, 22}, {19, 23}, {21, 29}, {24, 26}, {25, 30}, {31, 32}, {33, 35}, {37, 39},
                {3, 6}, {5, 8}, {7, 9}, {10, 16}, {12, 19}, {13, 15}, {14, 17}, {18, 21}, {20, 23}, {22, 29}, {24, 27}, {25, 31}, {26, 28}, {32, 34}, {33, 36}, {35, 38},
                {3, 4}, {6, 7}, {8, 11}, {12, 13}, {14, 19}, {15, 20}, {17, 23}, {18, 24}, {21, 26}, {22, 27}, {28, 29}, {30, 33}, {34, 35}, {37, 38},
                {5, 6}, {7, 8}, {9, 11}, {10, 12}, {13, 15}, {14, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 27}, {26, 28}, {29, 31}, {30, 32}, {33, 34}, {35, 36},
                {6, 7}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 19}, {18, 20}, {21, 23}, {22, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {34, 35},
                {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT42_H_
