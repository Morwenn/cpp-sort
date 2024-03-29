/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT46_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT46_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<46>
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
            iter_swap_if(first + 2, first + 13, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 14, first + 35, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 16, first + 29, compare, projection);
            iter_swap_if(first + 17, first + 28, compare, projection);
            iter_swap_if(first + 19, first + 30, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 45, compare, projection);
            iter_swap_if(first + 33, first + 44, compare, projection);
            iter_swap_if(first + 36, first + 40, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 7, compare, projection);
            iter_swap_if(first + 2, first + 9, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 36, compare, projection);
            iter_swap_if(first + 15, first + 25, compare, projection);
            iter_swap_if(first + 16, first + 21, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 26, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 29, compare, projection);
            iter_swap_if(first + 24, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 31, first + 41, compare, projection);
            iter_swap_if(first + 32, first + 37, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 34, first + 42, compare, projection);
            iter_swap_if(first + 35, first + 40, compare, projection);
            iter_swap_if(first + 38, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 31, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 27, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 32, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 43, compare, projection);
            iter_swap_if(first + 35, first + 38, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 42, compare, projection);
            iter_swap_if(first + 44, first + 45, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 14, first + 32, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 36, compare, projection);
            iter_swap_if(first + 35, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 43, compare, projection);
            iter_swap_if(first + 38, first + 41, compare, projection);
            iter_swap_if(first + 40, first + 44, compare, projection);
            iter_swap_if(first + 42, first + 45, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 26, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 45, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 40, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 37, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first, first + 14, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 26, first + 40, compare, projection);
            iter_swap_if(first + 27, first + 29, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 1, first + 31, compare, projection);
            iter_swap_if(first + 2, first + 32, compare, projection);
            iter_swap_if(first + 3, first + 33, compare, projection);
            iter_swap_if(first + 5, first + 36, compare, projection);
            iter_swap_if(first + 8, first + 21, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 39, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 12, first + 40, compare, projection);
            iter_swap_if(first + 13, first + 42, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 22, first + 35, compare, projection);
            iter_swap_if(first + 23, first + 38, compare, projection);
            iter_swap_if(first + 29, first + 44, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 4, first + 18, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 6, first + 22, compare, projection);
            iter_swap_if(first + 7, first + 38, compare, projection);
            iter_swap_if(first + 8, first + 37, compare, projection);
            iter_swap_if(first + 12, first + 26, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 17, first + 32, compare, projection);
            iter_swap_if(first + 19, first + 33, compare, projection);
            iter_swap_if(first + 24, first + 39, compare, projection);
            iter_swap_if(first + 25, first + 41, compare, projection);
            iter_swap_if(first + 27, first + 43, compare, projection);
            iter_swap_if(first + 2, first + 17, compare, projection);
            iter_swap_if(first + 4, first + 14, compare, projection);
            iter_swap_if(first + 5, first + 15, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 8, first + 19, compare, projection);
            iter_swap_if(first + 9, first + 25, compare, projection);
            iter_swap_if(first + 10, first + 24, compare, projection);
            iter_swap_if(first + 11, first + 27, compare, projection);
            iter_swap_if(first + 16, first + 31, compare, projection);
            iter_swap_if(first + 18, first + 34, compare, projection);
            iter_swap_if(first + 20, first + 36, compare, projection);
            iter_swap_if(first + 21, first + 37, compare, projection);
            iter_swap_if(first + 22, first + 35, compare, projection);
            iter_swap_if(first + 26, first + 38, compare, projection);
            iter_swap_if(first + 28, first + 42, compare, projection);
            iter_swap_if(first + 30, first + 39, compare, projection);
            iter_swap_if(first + 41, first + 44, compare, projection);
            iter_swap_if(first + 43, first + 45, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 14, compare, projection);
            iter_swap_if(first + 6, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 32, compare, projection);
            iter_swap_if(first + 9, first + 31, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 37, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 26, compare, projection);
            iter_swap_if(first + 29, first + 35, compare, projection);
            iter_swap_if(first + 30, first + 33, compare, projection);
            iter_swap_if(first + 39, first + 42, compare, projection);
            iter_swap_if(first + 40, first + 43, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 18, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 32, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 13, first + 34, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 29, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 24, first + 40, compare, projection);
            iter_swap_if(first + 25, first + 30, compare, projection);
            iter_swap_if(first + 27, first + 37, compare, projection);
            iter_swap_if(first + 28, first + 33, compare, projection);
            iter_swap_if(first + 35, first + 41, compare, projection);
            iter_swap_if(first + 42, first + 45, compare, projection);
            iter_swap_if(first + 43, first + 44, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 14, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 22, compare, projection);
            iter_swap_if(first + 11, first + 20, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 34, compare, projection);
            iter_swap_if(first + 25, first + 36, compare, projection);
            iter_swap_if(first + 29, first + 31, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 41, compare, projection);
            iter_swap_if(first + 37, first + 40, compare, projection);
            iter_swap_if(first + 39, first + 43, compare, projection);
            iter_swap_if(first + 42, first + 44, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 25, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 36, compare, projection);
            iter_swap_if(first + 24, first + 29, compare, projection);
            iter_swap_if(first + 26, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 32, first + 38, compare, projection);
            iter_swap_if(first + 35, first + 37, compare, projection);
            iter_swap_if(first + 39, first + 40, compare, projection);
            iter_swap_if(first + 42, first + 43, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 25, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 35, compare, projection);
            iter_swap_if(first + 28, first + 36, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 33, first + 39, compare, projection);
            iter_swap_if(first + 37, first + 38, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 31, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 32, first + 36, compare, projection);
            iter_swap_if(first + 33, first + 37, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 38, first + 41, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 32, first + 34, compare, projection);
            iter_swap_if(first + 35, first + 36, compare, projection);
            iter_swap_if(first + 40, first + 41, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 34, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 38, compare, projection);
            iter_swap_if(first + 41, first + 42, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 30, compare, projection);
            iter_swap_if(first + 29, first + 32, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 33, first + 35, compare, projection);
            iter_swap_if(first + 36, first + 37, compare, projection);
            iter_swap_if(first + 38, first + 39, compare, projection);
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
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 332>
        {
            return {{
                {2, 13}, {3, 12}, {4, 8}, {5, 6}, {7, 11}, {9, 10}, {14, 35}, {15, 18}, {16, 29}, {17, 28}, {19, 30}, {20, 24}, {21, 22}, {23, 27}, {25, 26}, {31, 34}, {32, 45}, {33, 44}, {36, 40}, {37, 38}, {39, 43}, {41, 42},
                {0, 5}, {1, 7}, {2, 9}, {3, 4}, {8, 12}, {10, 13}, {14, 36}, {15, 25}, {16, 21}, {17, 23}, {18, 26}, {19, 20}, {22, 29}, {24, 30}, {27, 28}, {31, 41}, {32, 37}, {33, 39}, {34, 42}, {35, 40}, {38, 45}, {43, 44},
                {0, 1}, {2, 3}, {4, 5}, {6, 8}, {7, 9}, {10, 11}, {12, 13}, {14, 31}, {15, 19}, {16, 17}, {18, 27}, {20, 21}, {22, 24}, {23, 25}, {26, 30}, {28, 29}, {32, 33}, {34, 43}, {35, 38}, {36, 37}, {39, 41}, {40, 42}, {44, 45},
                {0, 2}, {1, 3}, {4, 10}, {5, 11}, {6, 7}, {8, 9}, {14, 32}, {15, 16}, {17, 19}, {18, 20}, {21, 27}, {22, 23}, {24, 25}, {26, 28}, {29, 30}, {31, 33}, {34, 36}, {35, 39}, {37, 43}, {38, 41}, {40, 44}, {42, 45},
                {1, 2}, {3, 12}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {14, 15}, {16, 17}, {18, 22}, {19, 26}, {20, 24}, {21, 23}, {25, 27}, {28, 29}, {30, 45}, {31, 32}, {33, 40}, {34, 35}, {36, 38}, {37, 39}, {41, 43}, {42, 44},
                {0, 14}, {1, 4}, {2, 6}, {3, 19}, {5, 8}, {7, 10}, {9, 13}, {16, 18}, {17, 22}, {20, 21}, {23, 24}, {25, 28}, {26, 40}, {27, 29}, {31, 34}, {32, 35}, {36, 37}, {38, 39}, {41, 42}, {43, 44},
                {1, 31}, {2, 32}, {3, 33}, {5, 36}, {8, 21}, {9, 25}, {10, 39}, {11, 27}, {12, 40}, {13, 42}, {14, 15}, {18, 34}, {22, 35}, {23, 38}, {29, 44},
                {1, 16}, {4, 18}, {5, 20}, {6, 22}, {7, 38}, {8, 37}, {12, 26}, {13, 28}, {17, 32}, {19, 33}, {24, 39}, {25, 41}, {27, 43},
                {2, 17}, {4, 14}, {5, 15}, {7, 23}, {8, 19}, {9, 25}, {10, 24}, {11, 27}, {16, 31}, {18, 34}, {20, 36}, {21, 37}, {22, 35}, {26, 38}, {28, 42}, {30, 39}, {41, 44}, {43, 45},
                {1, 4}, {2, 5}, {3, 14}, {6, 16}, {7, 32}, {9, 31}, {12, 15}, {13, 37}, {19, 21}, {23, 26}, {29, 35}, {30, 33}, {39, 42}, {40, 43},
                {2, 4}, {3, 6}, {7, 18}, {9, 16}, {11, 32}, {12, 17}, {13, 34}, {14, 20}, {15, 22}, {21, 29}, {23, 31}, {24, 40}, {25, 30}, {27, 37}, {28, 33}, {35, 41}, {42, 45}, {43, 44},
                {3, 4}, {5, 6}, {7, 14}, {9, 12}, {10, 22}, {11, 20}, {13, 15}, {21, 23}, {24, 34}, {25, 36}, {29, 31}, {30, 32}, {33, 41}, {37, 40}, {39, 43}, {42, 44},
                {4, 5}, {6, 7}, {8, 13}, {10, 25}, {11, 21}, {14, 16}, {15, 19}, {20, 23}, {22, 36}, {24, 29}, {26, 30}, {31, 34}, {32, 38}, {35, 37}, {39, 40}, {42, 43},
                {7, 12}, {8, 14}, {10, 17}, {13, 15}, {16, 18}, {19, 25}, {20, 24}, {22, 26}, {23, 29}, {27, 35}, {28, 36}, {30, 32}, {33, 39}, {37, 38},
                {8, 9}, {10, 13}, {11, 16}, {12, 14}, {15, 17}, {18, 21}, {19, 22}, {25, 26}, {27, 31}, {28, 30}, {32, 36}, {33, 37}, {34, 35}, {38, 41},
                {6, 8}, {10, 11}, {13, 16}, {15, 18}, {17, 21}, {19, 20}, {22, 24}, {23, 25}, {26, 29}, {27, 28}, {30, 31}, {32, 34}, {35, 36}, {40, 41},
                {5, 6}, {9, 10}, {11, 13}, {14, 15}, {16, 18}, {17, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 32}, {31, 33}, {34, 35}, {36, 38}, {41, 42},
                {7, 9}, {10, 12}, {11, 14}, {13, 15}, {16, 17}, {18, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30}, {29, 32}, {31, 34}, {33, 35}, {36, 37}, {38, 39},
                {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28}, {29, 30}, {31, 32}, {33, 34}, {35, 36}, {37, 38}, {39, 40},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT46_H_
