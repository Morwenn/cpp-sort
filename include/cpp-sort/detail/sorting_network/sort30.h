/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT30_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT30_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<30>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            sorting_network_sorter<15>{}(first, first + 15, compare, projection);
            sorting_network_sorter<15>{}(first + 15, last, compare, projection);

            iter_swap_if(first + 14, first + 29, compare, projection);
            iter_swap_if(first, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 28, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 27, compare, projection);
            iter_swap_if(first + 2, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 19, compare, projection);
            iter_swap_if(first + 9, first + 24, compare, projection);
            iter_swap_if(first + 10, first + 25, compare, projection);
            iter_swap_if(first + 11, first + 26, compare, projection);
            iter_swap_if(first + 5, first + 20, compare, projection);
            iter_swap_if(first + 6, first + 21, compare, projection);
            iter_swap_if(first + 7, first + 22, compare, projection);
            iter_swap_if(first + 8, first + 23, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 12, first + 19, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 10, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 20, compare, projection);
            iter_swap_if(first + 14, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 14, first + 17, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 24, first + 26, compare, projection);
            iter_swap_if(first + 25, first + 27, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
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
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
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
