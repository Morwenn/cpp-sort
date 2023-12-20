/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_

namespace cppsort::detail
{
    template<>
    struct sorting_network_sorter_impl<28>
    {
        template<
            mstd::random_access_iterator Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires is_projection_iterator_v<Projection, Iterator, Compare>
        auto operator()(Iterator first, Iterator,
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
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first + 22, first + 26, compare, projection);
            iter_swap_if(first + 23, first + 27, compare, projection);
            iter_swap_if(first, first + 20, compare, projection);
            iter_swap_if(first + 1, first + 21, compare, projection);
            iter_swap_if(first + 2, first + 22, compare, projection);
            iter_swap_if(first + 3, first + 23, compare, projection);
            iter_swap_if(first + 4, first + 24, compare, projection);
            iter_swap_if(first + 5, first + 25, compare, projection);
            iter_swap_if(first + 6, first + 26, compare, projection);
            iter_swap_if(first + 7, first + 27, compare, projection);
            iter_swap_if(first + 9, first + 17, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 20, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 23, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 18, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 19, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 12, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 20, compare, projection);
            iter_swap_if(first + 15, first + 25, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 26, compare, projection);
            iter_swap_if(first + 19, first + 27, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 17, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 22, compare, projection);
            iter_swap_if(first + 13, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 1, first + 8, compare, projection);
            iter_swap_if(first + 3, first + 9, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 10, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 12, first + 20, compare, projection);
            iter_swap_if(first + 16, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 18, first + 24, compare, projection);
            iter_swap_if(first + 19, first + 26, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 25, first + 26, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 23, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 16, compare, projection);
            iter_swap_if(first + 9, first + 14, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 20, compare, projection);
            iter_swap_if(first + 13, first + 18, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first + 3, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 24, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
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
        }

        template<typename DifferenceType=std::ptrdiff_t>
        [[nodiscard]] static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 155>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25}, {26, 27},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {20, 22}, {21, 23}, {24, 26}, {25, 27},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {14, 18}, {15, 19}, {20, 24}, {21, 25}, {22, 26}, {23, 27},
                {0, 20}, {1, 21}, {2, 22}, {3, 23}, {4, 24}, {5, 25}, {6, 26}, {7, 27}, {9, 17}, {10, 18}, {11, 15}, {12, 16},
                {1, 2}, {4, 20}, {5, 6}, {7, 23}, {8, 12}, {9, 16}, {10, 14}, {11, 18}, {13, 17}, {15, 19}, {21, 22}, {25, 26},
                {0, 8}, {1, 9}, {2, 12}, {3, 5}, {4, 10}, {6, 16}, {7, 13}, {11, 21}, {14, 20}, {15, 25}, {17, 23}, {18, 26}, {19, 27}, {22, 24},
                {2, 4}, {3, 7}, {5, 17}, {8, 14}, {9, 11}, {10, 22}, {13, 19}, {16, 18}, {20, 24}, {23, 25},
                {1, 8}, {3, 9}, {5, 11}, {6, 10}, {7, 15}, {12, 20}, {16, 22}, {17, 21}, {18, 24}, {19, 26},
                {1, 2}, {4, 6}, {5, 9}, {10, 16}, {11, 17}, {12, 14}, {13, 15}, {18, 22}, {21, 23}, {25, 26},
                {4, 8}, {6, 12}, {7, 11}, {10, 14}, {13, 17}, {15, 21}, {16, 20}, {19, 23},
                {2, 4}, {6, 8}, {7, 16}, {9, 14}, {10, 12}, {11, 20}, {13, 18}, {15, 17}, {19, 21}, {23, 25},
                {3, 10}, {5, 12}, {7, 9}, {11, 13}, {14, 16}, {15, 22}, {17, 24}, {18, 20},
                {3, 6}, {5, 8}, {7, 10}, {9, 12}, {11, 14}, {13, 16}, {15, 18}, {17, 20}, {19, 22}, {21, 24},
                {3, 4}, {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20}, {21, 22}, {23, 24},
            }};
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT28_H_
