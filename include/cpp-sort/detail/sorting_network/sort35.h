/*
* Copyright (c) 2023 Morwenn
* SPDX-License-Identifier: MIT
*/
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT35_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT35_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<35>
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
            iter_swap_if(first + 33, first + 34, compare, projection);
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
            iter_swap_if(first + 32, first + 33, compare, projection);
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
            iter_swap_if(first, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 31, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 28, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 23, first + 29, compare, projection);
            iter_swap_if(first + 27, first + 30, compare, projection);
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
            iter_swap_if(first + 16, first + 32, compare, projection);
            iter_swap_if(first + 2, first + 16, compare, projection);
            iter_swap_if(first + 5, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 32, compare, projection);
            iter_swap_if(first + 17, first + 33, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 24, first + 34, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 6, first + 17, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 32, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 34, compare, projection);
            iter_swap_if(first + 25, first + 33, compare, projection);
            iter_swap_if(first + 3, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 11, first + 25, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 14, first + 34, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 22, first + 32, compare, projection);
            iter_swap_if(first + 26, first + 33, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 7, first + 24, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 26, compare, projection);
            iter_swap_if(first + 14, first + 32, compare, projection);
            iter_swap_if(first + 15, first + 33, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 28, compare, projection);
            iter_swap_if(first + 31, first + 34, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 18, compare, projection);
            iter_swap_if(first + 10, first + 20, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 13, first + 24, compare, projection);
            iter_swap_if(first + 14, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 28, compare, projection);
            iter_swap_if(first + 23, first + 31, compare, projection);
            iter_swap_if(first + 25, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 33, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 24, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 32, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 22, first + 25, compare, projection);
            iter_swap_if(first + 27, first + 32, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 33, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 7, first + 14, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
            iter_swap_if(first + 30, first + 32, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 15, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 21, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 28, compare, projection);
            iter_swap_if(first + 29, first + 30, compare, projection);
            iter_swap_if(first + 31, first + 32, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 12, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 27, first + 28, compare, projection);
            iter_swap_if(first + 30, first + 31, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 26, first + 27, compare, projection);
            iter_swap_if(first + 28, first + 29, compare, projection);
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
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 220>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27}, {28, 29}, {30, 31}, {32, 33},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27}, {28, 30}, {29, 31}, {33, 34},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {16, 20}, {17, 21}, {18, 22}, {19, 23}, {24, 28}, {25, 29}, {26, 30}, {27, 31}, {32, 33},
                {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}, {16, 24}, {17, 25}, {18, 26}, {19, 27}, {20, 28}, {21, 29}, {22, 30}, {23, 31},
                {0, 16}, {1, 4}, {2, 8}, {3, 12}, {5, 10}, {6, 9}, {7, 13}, {11, 14}, {15, 31}, {17, 20}, {18, 24}, {19, 28}, {21, 26}, {22, 25}, {23, 29}, {27, 30},
                {1, 17}, {2, 18}, {3, 19}, {4, 20}, {5, 21}, {6, 22}, {7, 23}, {8, 24}, {9, 25}, {10, 26}, {11, 27}, {12, 28}, {13, 29}, {14, 30}, {16, 32},
                {2, 16}, {5, 18}, {7, 11}, {8, 32}, {17, 33}, {19, 21}, {24, 34}, {25, 28},
                {0, 2}, {6, 17}, {8, 16}, {10, 32}, {12, 19}, {20, 34}, {25, 33},
                {3, 10}, {5, 6}, {11, 25}, {12, 17}, {14, 34}, {20, 24}, {22, 32}, {26, 33},
                {3, 5}, {4, 20}, {7, 24}, {9, 11}, {13, 26}, {14, 32}, {15, 33}, {19, 22}, {25, 28}, {31, 34},
                {1, 3}, {4, 8}, {9, 18}, {10, 20}, {11, 21}, {13, 24}, {14, 22}, {15, 28}, {23, 31}, {25, 32}, {29, 33},
                {1, 2}, {5, 10}, {6, 9}, {8, 16}, {11, 20}, {14, 19}, {17, 18}, {21, 22}, {23, 27}, {24, 25}, {26, 32}, {30, 31},
                {2, 4}, {3, 8}, {5, 6}, {7, 11}, {9, 10}, {12, 16}, {13, 20}, {14, 17}, {15, 23}, {18, 19}, {22, 25}, {27, 32}, {29, 30}, {31, 33},
                {3, 4}, {7, 14}, {8, 12}, {9, 16}, {11, 18}, {13, 17}, {15, 21}, {19, 20}, {23, 27}, {28, 29}, {30, 32},
                {5, 8}, {6, 12}, {10, 16}, {11, 13}, {15, 24}, {17, 18}, {21, 26}, {23, 28}, {29, 30}, {31, 32},
                {6, 8}, {7, 12}, {14, 16}, {15, 19}, {20, 21}, {22, 24}, {25, 26}, {27, 28}, {30, 31},
                {7, 9}, {10, 12}, {11, 14}, {13, 16}, {15, 17}, {18, 19}, {20, 22}, {21, 24}, {23, 25}, {26, 27}, {28, 29},
                {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24}, {25, 26}, {27, 28},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT35_H_
