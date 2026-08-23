/*
 * Copyright (c) 2015-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT26_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT26_H_

namespace cppsort::detail
{
    template<>
    struct sorting_network_sorter_impl<26>
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
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first + 23, first + 25, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 14, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 10, first + 15, compare, projection);
            iter_swap_if(first + 11, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 21, first + 25, compare, projection);
            iter_swap_if(first, first + 18, compare, projection);
            iter_swap_if(first + 1, first + 19, compare, projection);
            iter_swap_if(first + 2, first + 20, compare, projection);
            iter_swap_if(first + 3, first + 21, compare, projection);
            iter_swap_if(first + 4, first + 22, compare, projection);
            iter_swap_if(first + 5, first + 23, compare, projection);
            iter_swap_if(first + 6, first + 24, compare, projection);
            iter_swap_if(first + 7, first + 25, compare, projection);
            iter_swap_if(first + 9, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 16, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 12, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 22, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 14, compare, projection);
            iter_swap_if(first + 6, first + 12, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 10, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 23, compare, projection);
            iter_swap_if(first + 13, first + 19, compare, projection);
            iter_swap_if(first + 16, first + 24, compare, projection);
            iter_swap_if(first + 17, first + 25, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 22, compare, projection);
            iter_swap_if(first + 17, first + 21, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first + 3, first + 14, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 5, first + 18, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 8, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 22, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 15, first + 21, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 16, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 21, first + 24, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 19, compare, projection);
            iter_swap_if(first + 12, first + 16, compare, projection);
            iter_swap_if(first + 15, first + 22, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 2, first + 8, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 16, first + 19, compare, projection);
            iter_swap_if(first + 17, first + 23, compare, projection);
            iter_swap_if(first + 18, first + 20, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 11, compare, projection);
            iter_swap_if(first + 7, first + 10, compare, projection);
            iter_swap_if(first + 9, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 20, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 10, first + 11, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 17, compare, projection);
            iter_swap_if(first + 18, first + 19, compare, projection);
            iter_swap_if(first + 20, first + 21, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        [[nodiscard]]
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 138>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23}, {24, 25},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {14, 16}, {15, 17}, {18, 20}, {19, 21}, {22, 24}, {23, 25},
                {0, 4}, {1, 6}, {2, 5}, {3, 7}, {8, 14}, {9, 16}, {10, 15}, {11, 17}, {18, 22}, {19, 24}, {20, 23}, {21, 25},
                {0, 18}, {1, 19}, {2, 20}, {3, 21}, {4, 22}, {5, 23}, {6, 24}, {7, 25}, {9, 12}, {13, 16},
                {3, 11}, {8, 9}, {10, 13}, {12, 15}, {14, 22}, {16, 17},
                {0, 8}, {1, 9}, {2, 14}, {6, 12}, {7, 15}, {10, 18}, {11, 23}, {13, 19}, {16, 24}, {17, 25},
                {1, 2}, {3, 18}, {4, 8}, {7, 22}, {17, 21}, {23, 24},
                {3, 14}, {4, 10}, {5, 18}, {7, 20}, {8, 13}, {11, 22}, {12, 17}, {15, 21},
                {1, 4}, {5, 6}, {7, 9}, {8, 10}, {15, 17}, {16, 18}, {19, 20}, {21, 24},
                {2, 5}, {3, 10}, {6, 14}, {9, 13}, {11, 19}, {12, 16}, {15, 22}, {20, 23},
                {2, 8}, {5, 7}, {6, 9}, {11, 12}, {13, 14}, {16, 19}, {17, 23}, {18, 20},
                {2, 4}, {3, 5}, {6, 11}, {7, 10}, {9, 16}, {12, 13}, {14, 19}, {15, 18}, {20, 22}, {21, 23},
                {3, 4}, {5, 8}, {6, 7}, {9, 11}, {10, 12}, {13, 15}, {14, 16}, {17, 20}, {18, 19}, {21, 22},
                {5, 6}, {7, 8}, {9, 10}, {11, 12}, {13, 14}, {15, 16}, {17, 18}, {19, 20},
                {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21},
            }};
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT26_H_
