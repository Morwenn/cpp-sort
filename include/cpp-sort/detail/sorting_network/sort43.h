/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT43_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT43_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<43>
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
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 37, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 20, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 36, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first, first + 31, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 24, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 42, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 32, compare, projection);
            iter_swap_if(first + 12, first + 23, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 30, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 38, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 36, compare, projection);
            iter_swap_if(first + 3, first + 27, compare, projection);
            iter_swap_if(first + 5, first + 25, compare, projection);
            iter_swap_if(first + 6, first + 41, compare, projection);
            iter_swap_if(first + 7, first + 19, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 10, first + 21, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 16, first + 39, compare, projection);
            iter_swap_if(first + 18, first + 37, compare, projection);
            iter_swap_if(first + 20, first + 33, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 24, first + 31, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 30, first + 35, compare, projection);
            iter_swap_if(first + 38, first + 42, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 26, compare, projection);
            iter_swap_if(first + 3, first + 15, compare, projection);
            iter_swap_if(first + 4, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 30, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 16, first + 23, compare, projection);
            iter_swap_if(first + 17, first + 40, compare, projection);
            iter_swap_if(first + 21, first + 29, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 36, compare, projection);
            iter_swap_if(first + 28, first + 33, compare, projection);
            iter_swap_if(first + 32, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 2, first + 10, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 9, first + 20, compare, projection);
            iter_swap_if(first + 11, first + 23, compare, projection);
            iter_swap_if(first + 12, first + 31, compare, projection);
            iter_swap_if(first + 14, first + 26, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 34, compare, projection);
            iter_swap_if(first + 22, first + 38, compare, projection);
            iter_swap_if(first + 24, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 42, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 21, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 35, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 23, first + 32, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 41, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 24, compare, projection);
            iter_swap_if(first + 16, first + 23, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 19, first + 38, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 35, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 41, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 25, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 18, first + 29, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 26, first + 34, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 42, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 32, compare, projection);
            iter_swap_if(first + 7, first + 36, compare, projection);
            iter_swap_if(first + 8, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 25, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 26, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 33, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 32, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 39, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 27, first + 36, compare, projection);
            iter_swap_if(first + 29, first + 34, compare, projection);
            iter_swap_if(first + 30, first + 41, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 31, compare, projection);
            iter_swap_if(first + 6, first + 11, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 9, first + 23, compare, projection);
            iter_swap_if(first + 14, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 39, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 32, compare, projection);
            iter_swap_if(first + 22, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 23, compare, projection);
            iter_swap_if(first + 15, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 31, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 33, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 8, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 31, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 30, first + 39, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 31, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
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
            iter_swap_if(first + 37, first + 38, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 304>
        {
            return {{
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {9, 10}, {11, 21}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {23, 29}, {24, 26}, {25, 27}, {28, 30}, {31, 37}, {32, 34}, {33, 35}, {36, 38}, {39, 41}, {40, 42},
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {11, 20}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {21, 22}, {23, 28}, {24, 25}, {26, 27}, {29, 30}, {31, 36}, {32, 33}, {34, 35}, {37, 38}, {39, 40}, {41, 42},
                {0, 31}, {1, 2}, {4, 24}, {5, 6}, {7, 42}, {8, 16}, {9, 10}, {11, 32}, {12, 23}, {13, 14}, {15, 30}, {17, 18}, {19, 38}, {20, 21}, {25, 26}, {27, 35}, {28, 29}, {33, 34}, {36, 37}, {40, 41},
                {0, 4}, {1, 36}, {3, 27}, {5, 25}, {6, 41}, {7, 19}, {9, 17}, {10, 21}, {11, 12}, {13, 28}, {14, 29}, {15, 22}, {16, 39}, {18, 37}, {20, 33}, {23, 32}, {24, 31}, {26, 34}, {30, 35}, {38, 42},
                {1, 5}, {2, 26}, {3, 15}, {4, 11}, {6, 18}, {7, 30}, {8, 12}, {10, 14}, {13, 20}, {16, 23}, {17, 40}, {21, 29}, {22, 27}, {25, 36}, {28, 33}, {32, 39}, {37, 41},
                {0, 16}, {2, 10}, {3, 7}, {4, 8}, {5, 13}, {9, 20}, {11, 23}, {12, 31}, {14, 26}, {15, 19}, {17, 28}, {21, 34}, {22, 38}, {24, 32}, {27, 42}, {29, 37}, {33, 40},
                {0, 4}, {1, 17}, {5, 9}, {6, 21}, {8, 24}, {10, 18}, {12, 16}, {13, 28}, {14, 29}, {15, 22}, {19, 35}, {20, 36}, {23, 32}, {25, 33}, {26, 41}, {27, 30}, {31, 39}, {34, 37},
                {1, 5}, {2, 6}, {7, 15}, {8, 12}, {9, 25}, {10, 14}, {11, 24}, {16, 23}, {17, 20}, {18, 34}, {19, 38}, {21, 26}, {22, 27}, {28, 33}, {30, 35}, {31, 32}, {36, 40}, {37, 41},
                {4, 8}, {6, 10}, {9, 17}, {11, 12}, {13, 25}, {14, 21}, {15, 22}, {16, 24}, {18, 29}, {19, 27}, {20, 28}, {23, 31}, {26, 34}, {30, 38}, {33, 36}, {35, 42},
                {1, 4}, {5, 9}, {6, 32}, {7, 36}, {8, 11}, {10, 14}, {12, 16}, {13, 17}, {18, 21}, {19, 22}, {20, 25}, {23, 24}, {26, 29}, {27, 30}, {28, 33}, {34, 37}, {35, 38}, {41, 42},
                {2, 8}, {3, 33}, {4, 12}, {5, 11}, {6, 16}, {7, 32}, {9, 13}, {10, 39}, {14, 18}, {17, 20}, {21, 26}, {25, 28}, {27, 36}, {29, 34}, {30, 41}, {35, 37}, {38, 40},
                {2, 4}, {3, 31}, {6, 11}, {7, 20}, {9, 23}, {14, 24}, {15, 39}, {19, 28}, {21, 32}, {22, 34}, {35, 36}, {40, 41},
                {3, 8}, {7, 14}, {10, 23}, {15, 25}, {18, 31}, {19, 20}, {21, 24}, {22, 33}, {28, 32}, {38, 39},
                {3, 12}, {7, 13}, {8, 17}, {15, 23}, {19, 21}, {20, 24}, {22, 31}, {26, 38}, {29, 32}, {30, 39},
                {3, 5}, {7, 11}, {8, 12}, {13, 16}, {17, 18}, {25, 26}, {27, 29}, {30, 38}, {32, 35}, {37, 39},
                {3, 4}, {8, 9}, {10, 12}, {11, 13}, {14, 16}, {15, 17}, {18, 23}, {22, 25}, {26, 31}, {27, 28}, {29, 32}, {30, 33}, {34, 38}, {39, 40},
                {5, 8}, {9, 10}, {12, 15}, {16, 19}, {17, 18}, {20, 22}, {21, 23}, {24, 27}, {25, 26}, {30, 31}, {33, 34}, {37, 38},
                {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 30}, {29, 31}, {32, 33}, {34, 35}, {36, 37},
                {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT43_H_
