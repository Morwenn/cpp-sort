/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT48_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT48_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<48>
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
            iter_swap_if(first, first + 13, compare, projection);
            iter_swap_if(first + 1, first + 12, compare, projection);
            iter_swap_if(first + 2, first + 15, compare, projection);
            iter_swap_if(first + 3, first + 14, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 16, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 28, compare, projection);
            iter_swap_if(first + 18, first + 31, compare, projection);
            iter_swap_if(first + 19, first + 30, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 32, first + 45, compare, projection);
            iter_swap_if(first + 33, first + 44, compare, projection);
            iter_swap_if(first + 34, first + 47, compare, projection);
            iter_swap_if(first + 35, first + 46, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 13, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 25, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 30, compare, projection);
            iter_swap_if(first + 26, first + 31, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 32, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 41, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 38, first + 45, compare, projection);
            iter_swap_if(first + 40, first + 46, compare, projection);
            iter_swap_if(first + 42, first + 47, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first + 46, first + 47, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 26, compare, projection);
            iter_swap_if(first + 21, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 42, compare, projection);
            iter_swap_if(first + 37, first + 43, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 44, first + 46, compare, projection);
            iter_swap_if(first + 45, first + 47, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 33, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 44, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 45, first + 46, compare, projection);
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 44, compare, projection);
            iter_swap_if(first + 31, first + 47, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 34, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 41, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 1, first + 33, compare, projection);
            iter_swap_if(first + 2, first + 34, compare, projection);
            iter_swap_if(first + 3, first + 35, compare, projection);
            iter_swap_if(first + 5, first + 37, compare, projection);
            iter_swap_if(first + 8, first + 24, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 42, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 12, first + 44, compare, projection);
            iter_swap_if(first + 13, first + 45, compare, projection);
            iter_swap_if(first + 14, first + 46, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 22, first + 38, compare, projection);
            iter_swap_if(first + 23, first + 39, compare, projection);
            iter_swap_if(first + 1, first + 17, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 21, compare, projection);
            iter_swap_if(first + 6, first + 22, compare, projection);
            iter_swap_if(first + 7, first + 39, compare, projection);
            iter_swap_if(first + 8, first + 40, compare, projection);
            iter_swap_if(first + 12, first + 28, compare, projection);
            iter_swap_if(first + 13, first + 29, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 19, first + 35, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 26, first + 42, compare, projection);
            iter_swap_if(first + 27, first + 43, compare, projection);
            iter_swap_if(first + 30, first + 46, compare, projection);
            iter_swap_if(first + 2, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 32, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 8, first + 19, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 26, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 14, first + 30, compare, projection);
            iter_swap_if(first + 15, first + 42, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 21, first + 37, compare, projection);
            iter_swap_if(first + 22, first + 38, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 28, first + 39, compare, projection);
            iter_swap_if(first + 29, first + 45, compare, projection);
            iter_swap_if(first + 31, first + 43, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 16, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 7, first + 34, compare, projection);
            iter_swap_if(first + 9, first + 33, compare, projection);
            iter_swap_if(first + 12, first + 32, compare, projection);
            iter_swap_if(first + 13, first + 40, compare, projection);
            iter_swap_if(first + 14, first + 38, compare, projection);
            iter_swap_if(first + 15, first + 35, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 41, compare, projection);
            iter_swap_if(first + 31, first + 44, compare, projection);
            iter_swap_if(first + 42, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 46, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 34, compare, projection);
            iter_swap_if(first + 12, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 36, compare, projection);
            iter_swap_if(first + 14, first + 24, compare, projection);
            iter_swap_if(first + 15, first + 25, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 23, first + 33, compare, projection);
            iter_swap_if(first + 26, first + 31, compare, projection);
            iter_swap_if(first + 27, first + 40, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 30, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 32, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 13, first + 22, compare, projection);
            iter_swap_if(first + 14, first + 23, compare, projection);
            iter_swap_if(first + 15, first + 37, compare, projection);
            iter_swap_if(first + 24, first + 33, compare, projection);
            iter_swap_if(first + 25, first + 34, compare, projection);
            iter_swap_if(first + 26, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 40, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 34, first + 39, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 19, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 34, compare, projection);
            iter_swap_if(first + 29, first + 37, compare, projection);
            iter_swap_if(first + 31, first + 39, compare, projection);
            iter_swap_if(first + 35, first + 40, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 25, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 33, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 36, compare, projection);
            iter_swap_if(first + 31, first + 35, compare, projection);
            iter_swap_if(first + 34, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 30, first + 34, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 40, compare, projection);
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
            iter_swap_if(first + 39, first + 40, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 346>
        {
            return {{
                {0, 13}, {1, 12}, {2, 15}, {3, 14}, {4, 8}, {5, 6}, {7, 11}, {9, 10}, {16, 29}, {17, 28}, {18, 31}, {19, 30}, {20, 24}, {21, 22}, {23, 27}, {25, 26}, {32, 45}, {33, 44}, {34, 47}, {35, 46}, {36, 40}, {37, 38}, {39, 43}, {41, 42},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {6, 13}, {8, 14}, {10, 15}, {11, 12}, {16, 21}, {17, 23}, {18, 25}, {19, 20}, {22, 29}, {24, 30}, {26, 31}, {27, 28}, {32, 37}, {33, 39}, {34, 41}, {35, 36}, {38, 45}, {40, 46}, {42, 47}, {43, 44},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 24}, {23, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33}, {34, 35}, {36, 37}, {38, 40}, {39, 41}, {42, 43}, {44, 45}, {46, 47},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 26}, {21, 27}, {22, 23}, {24, 25}, {28, 30}, {29, 31}, {32, 34}, {33, 35}, {36, 42}, {37, 43}, {38, 39}, {40, 41}, {44, 46}, {45, 47},
                {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {13, 14}, {15, 31}, {16, 32}, {17, 18}, {19, 28}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {29, 30}, {33, 34}, {35, 44}, {36, 38}, {37, 39}, {40, 42}, {41, 43}, {45, 46},
                {0, 16}, {1, 4}, {2, 6}, {3, 19}, {5, 8}, {7, 10}, {9, 13}, {11, 14}, {17, 20}, {18, 22}, {21, 24}, {23, 26}, {25, 29}, {27, 30}, {28, 44}, {31, 47}, {33, 36}, {34, 38}, {37, 40}, {39, 42}, {41, 45}, {43, 46},
                {1, 33}, {2, 34}, {3, 35}, {5, 37}, {8, 24}, {9, 25}, {10, 42}, {11, 27}, {12, 44}, {13, 45}, {14, 46}, {15, 31}, {16, 32}, {20, 36}, {22, 38}, {23, 39},
                {1, 17}, {4, 20}, {5, 21}, {6, 22}, {7, 39}, {8, 40}, {12, 28}, {13, 29}, {18, 34}, {19, 35}, {25, 41}, {26, 42}, {27, 43}, {30, 46},
                {2, 18}, {4, 16}, {5, 32}, {7, 23}, {8, 19}, {9, 25}, {10, 26}, {11, 27}, {14, 30}, {15, 42}, {17, 33}, {20, 36}, {21, 37}, {22, 38}, {24, 40}, {28, 39}, {29, 45}, {31, 43},
                {1, 4}, {2, 5}, {3, 16}, {6, 17}, {7, 34}, {9, 33}, {12, 32}, {13, 40}, {14, 38}, {15, 35}, {19, 24}, {23, 28}, {30, 41}, {31, 44}, {42, 45}, {43, 46},
                {2, 4}, {3, 6}, {7, 20}, {9, 17}, {11, 34}, {12, 18}, {13, 36}, {14, 24}, {15, 25}, {16, 21}, {22, 32}, {23, 33}, {26, 31}, {27, 40}, {29, 35}, {30, 38}, {41, 44}, {43, 45},
                {3, 4}, {5, 6}, {7, 16}, {9, 12}, {10, 32}, {11, 21}, {13, 22}, {14, 23}, {15, 37}, {24, 33}, {25, 34}, {26, 36}, {31, 40}, {35, 38}, {41, 42}, {43, 44},
                {4, 5}, {6, 7}, {8, 13}, {10, 15}, {11, 14}, {16, 17}, {19, 22}, {21, 23}, {24, 26}, {25, 28}, {30, 31}, {32, 37}, {33, 36}, {34, 39}, {40, 41}, {42, 43},
                {7, 12}, {8, 16}, {10, 18}, {13, 19}, {15, 22}, {17, 20}, {21, 24}, {23, 26}, {25, 32}, {27, 30}, {28, 34}, {29, 37}, {31, 39}, {35, 40},
                {8, 9}, {10, 13}, {11, 17}, {12, 16}, {14, 20}, {15, 25}, {18, 19}, {22, 32}, {27, 33}, {28, 29}, {30, 36}, {31, 35}, {34, 37}, {38, 39},
                {6, 8}, {10, 11}, {13, 17}, {14, 18}, {15, 21}, {19, 20}, {22, 23}, {24, 25}, {26, 32}, {27, 28}, {29, 33}, {30, 34}, {36, 37}, {39, 41},
                {5, 6}, {9, 10}, {11, 13}, {14, 16}, {15, 19}, {17, 18}, {20, 21}, {22, 24}, {23, 25}, {26, 27}, {28, 32}, {29, 30}, {31, 33}, {34, 36}, {37, 38}, {41, 42},
                {7, 9}, {10, 12}, {11, 14}, {13, 16}, {15, 17}, {18, 19}, {20, 22}, {21, 24}, {23, 26}, {25, 27}, {28, 29}, {30, 32}, {31, 34}, {33, 36}, {35, 37}, {38, 40},
                {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT48_H_
