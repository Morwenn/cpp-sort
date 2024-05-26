/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<25>
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
            iter_swap_if(first + 21, first + 22, compare, projection);
            iter_swap_if(first + 23, first + 24, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 5, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 12, compare, projection);
            iter_swap_if(first + 9, first + 13, compare, projection);
            iter_swap_if(first + 10, first + 14, compare, projection);
            iter_swap_if(first + 11, first + 15, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 23, compare, projection);
            iter_swap_if(first + 22, first + 24, compare, projection);
            iter_swap_if(first, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 9, compare, projection);
            iter_swap_if(first + 2, first + 10, compare, projection);
            iter_swap_if(first + 3, first + 11, compare, projection);
            iter_swap_if(first + 4, first + 12, compare, projection);
            iter_swap_if(first + 5, first + 13, compare, projection);
            iter_swap_if(first + 6, first + 14, compare, projection);
            iter_swap_if(first + 7, first + 15, compare, projection);
            iter_swap_if(first + 16, first + 20, compare, projection);
            iter_swap_if(first + 17, first + 22, compare, projection);
            iter_swap_if(first + 19, first + 24, compare, projection);
            iter_swap_if(first + 21, first + 23, compare, projection);
            iter_swap_if(first + 1, first + 18, compare, projection);
            iter_swap_if(first + 3, first + 21, compare, projection);
            iter_swap_if(first + 5, first + 23, compare, projection);
            iter_swap_if(first + 6, first + 19, compare, projection);
            iter_swap_if(first + 11, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 24, compare, projection);
            iter_swap_if(first + 1, first + 16, compare, projection);
            iter_swap_if(first + 3, first + 17, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 11, compare, projection);
            iter_swap_if(first + 13, first + 19, compare, projection);
            iter_swap_if(first + 14, first + 23, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 16, compare, projection);
            iter_swap_if(first + 3, first + 8, compare, projection);
            iter_swap_if(first + 7, first + 20, compare, projection);
            iter_swap_if(first + 10, first + 13, compare, projection);
            iter_swap_if(first + 11, first + 22, compare, projection);
            iter_swap_if(first + 15, first + 23, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 5, first + 10, compare, projection);
            iter_swap_if(first + 7, first + 18, compare, projection);
            iter_swap_if(first + 11, first + 21, compare, projection);
            iter_swap_if(first + 15, first + 20, compare, projection);
            iter_swap_if(first + 19, first + 22, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 9, first + 18, compare, projection);
            iter_swap_if(first + 10, first + 17, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 21, compare, projection);
            iter_swap_if(first + 14, first + 15, compare, projection);
            iter_swap_if(first + 19, first + 20, compare, projection);
            iter_swap_if(first + 22, first + 23, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 17, compare, projection);
            iter_swap_if(first + 13, first + 18, compare, projection);
            iter_swap_if(first + 19, first + 21, compare, projection);
            iter_swap_if(first + 20, first + 22, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 11, compare, projection);
            iter_swap_if(first + 6, first + 16, compare, projection);
            iter_swap_if(first + 7, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first + 12, first + 13, compare, projection);
            iter_swap_if(first + 14, first + 19, compare, projection);
            iter_swap_if(first + 15, first + 18, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 8, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 16, compare, projection);
            iter_swap_if(first + 12, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 17, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 11, compare, projection);
            iter_swap_if(first + 10, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 14, compare, projection);
            iter_swap_if(first + 15, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 18, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 11, first + 12, compare, projection);
            iter_swap_if(first + 13, first + 15, compare, projection);
            iter_swap_if(first + 14, first + 16, compare, projection);
            iter_swap_if(first + 17, first + 19, compare, projection);
            iter_swap_if(first + 18, first + 21, compare, projection);
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
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 130>
        {
            return {{
                {0, 1}, {2, 3}, {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21}, {22, 23},
                {0, 2}, {1, 3}, {4, 6}, {5, 7}, {8, 10}, {9, 11}, {12, 14}, {13, 15}, {16, 18}, {17, 19}, {21, 22}, {23, 24},
                {0, 4}, {1, 5}, {2, 6}, {3, 7}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {18, 21}, {20, 23}, {22, 24},
                {0, 8}, {1, 9}, {2, 10}, {3, 11}, {4, 12}, {5, 13}, {6, 14}, {7, 15}, {16, 20}, {17, 22}, {19, 24}, {21, 23},
                {1, 18}, {3, 21}, {5, 23}, {6, 19}, {11, 14}, {15, 24},
                {1, 16}, {3, 17}, {6, 9}, {7, 11}, {13, 19}, {14, 23},
                {0, 1}, {2, 16}, {3, 8}, {7, 20}, {10, 13}, {11, 22}, {15, 23},
                {1, 2}, {5, 10}, {7, 18}, {11, 21}, {15, 20}, {19, 22},
                {4, 7}, {5, 6}, {9, 18}, {10, 17}, {11, 12}, {13, 21}, {14, 15}, {19, 20}, {22, 23},
                {3, 4}, {7, 8}, {9, 10}, {11, 16}, {12, 17}, {13, 18}, {19, 21}, {20, 22},
                {1, 3}, {2, 4}, {5, 11}, {6, 16}, {7, 9}, {8, 10}, {12, 13}, {14, 19}, {15, 18},
                {2, 3}, {5, 7}, {6, 9}, {8, 11}, {10, 16}, {12, 14}, {15, 17},
                {3, 5}, {4, 6}, {7, 8}, {9, 11}, {10, 12}, {13, 14}, {15, 16}, {17, 18},
                {4, 7}, {6, 8}, {9, 10}, {11, 12}, {13, 15}, {14, 16}, {17, 19}, {18, 21},
                {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17}, {18, 19}, {20, 21},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT25_H_
