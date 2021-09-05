/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT21_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT21_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<21u>
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
            iter_swap_if(first, first + 7u, compare, projection);
            iter_swap_if(first + 1u, first + 10u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 4u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 13u, compare, projection);
            iter_swap_if(first + 9u, first + 19u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 12u, first + 17u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 18u, first + 20u, compare, projection);
            iter_swap_if(first, first + 11u, compare, projection);
            iter_swap_if(first + 1u, first + 15u, compare, projection);
            iter_swap_if(first + 2u, first + 12u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first + 7u, first + 14u, compare, projection);
            iter_swap_if(first + 10u, first + 16u, compare, projection);
            iter_swap_if(first + 13u, first + 19u, compare, projection);
            iter_swap_if(first + 17u, first + 20u, compare, projection);
            iter_swap_if(first, first + 6u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 2u, first + 18u, compare, projection);
            iter_swap_if(first + 4u, first + 15u, compare, projection);
            iter_swap_if(first + 5u, first + 10u, compare, projection);
            iter_swap_if(first + 8u, first + 16u, compare, projection);
            iter_swap_if(first + 11u, first + 17u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 20u, compare, projection);
            iter_swap_if(first + 2u, first + 6u, compare, projection);
            iter_swap_if(first + 5u, first + 12u, compare, projection);
            iter_swap_if(first + 7u, first + 18u, compare, projection);
            iter_swap_if(first + 8u, first + 14u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 10u, first + 17u, compare, projection);
            iter_swap_if(first + 13u, first + 19u, compare, projection);
            iter_swap_if(first + 16u, first + 20u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 7u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 6u, first + 17u, compare, projection);
            iter_swap_if(first + 10u, first + 13u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 14u, first + 19u, compare, projection);
            iter_swap_if(first + 15u, first + 18u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 6u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 7u, first + 10u, compare, projection);
            iter_swap_if(first + 8u, first + 11u, compare, projection);
            iter_swap_if(first + 9u, first + 15u, compare, projection);
            iter_swap_if(first + 12u, first + 16u, compare, projection);
            iter_swap_if(first + 13u, first + 18u, compare, projection);
            iter_swap_if(first + 14u, first + 17u, compare, projection);
            iter_swap_if(first + 19u, first + 20u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 9u, compare, projection);
            iter_swap_if(first + 6u, first + 12u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 11u, first + 14u, compare, projection);
            iter_swap_if(first + 13u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 19u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 9u, compare, projection);
            iter_swap_if(first + 6u, first + 13u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
            iter_swap_if(first + 18u, first + 19u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 2u, first + 5u, compare, projection);
            iter_swap_if(first + 3u, first + 7u, compare, projection);
            iter_swap_if(first + 6u, first + 10u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 11u, first + 12u, compare, projection);
            iter_swap_if(first + 13u, first + 14u, compare, projection);
            iter_swap_if(first + 17u, first + 18u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 11u, compare, projection);
            iter_swap_if(first + 10u, first + 13u, compare, projection);
            iter_swap_if(first + 12u, first + 15u, compare, projection);
            iter_swap_if(first + 14u, first + 16u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 9u, first + 10u, compare, projection);
            iter_swap_if(first + 11u, first + 13u, compare, projection);
            iter_swap_if(first + 12u, first + 14u, compare, projection);
            iter_swap_if(first + 15u, first + 16u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 10u, first + 11u, compare, projection);
            iter_swap_if(first + 12u, first + 13u, compare, projection);
            iter_swap_if(first + 14u, first + 15u, compare, projection);
            iter_swap_if(first + 16u, first + 17u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 100>
        {
            return {{
                {0, 7}, {1, 10}, {3, 5}, {4, 8}, {6, 13}, {9, 19}, {11, 14}, {12, 17}, {15, 16}, {18, 20},
                {0, 11}, {1, 15}, {2, 12}, {3, 4}, {5, 8}, {6, 9}, {7, 14}, {10, 16}, {13, 19}, {17, 20},
                {0, 6}, {1, 3}, {2, 18}, {4, 15}, {5, 10}, {8, 16}, {11, 17}, {12, 13}, {14, 20},
                {2, 6}, {5, 12}, {7, 18}, {8, 14}, {9, 11}, {10, 17}, {13, 19}, {16, 20},
                {1, 2}, {4, 7}, {5, 9}, {6, 17}, {10, 13}, {11, 12}, {14, 19}, {15, 18},
                {0, 2}, {3, 6}, {4, 5}, {7, 10}, {8, 11}, {9, 15}, {12, 16}, {13, 18}, {14, 17}, {19, 20},
                {0, 1}, {2, 3}, {5, 9}, {6, 12}, {7, 8}, {11, 14}, {13, 15}, {16, 19}, {17, 18},
                {1, 2}, {3, 9}, {6, 13}, {10, 11}, {12, 15}, {16, 17}, {18, 19},
                {1, 4}, {2, 5}, {3, 7}, {6, 10}, {8, 9}, {11, 12}, {13, 14}, {17, 18},
                {2, 4}, {5, 6}, {7, 8}, {9, 11}, {10, 13}, {12, 15}, {14, 16},
                {3, 4}, {5, 7}, {6, 8}, {9, 10}, {11, 13}, {12, 14}, {15, 16},
                {4, 5}, {6, 7}, {8, 9}, {10, 11}, {12, 13}, {14, 15}, {16, 17},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT21_H_
