/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<10u>
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
            iter_swap_if(first + 4u, first + 9u, compare, projection);
            iter_swap_if(first + 3u, first + 8u, compare, projection);
            iter_swap_if(first + 2u, first + 7u, compare, projection);
            iter_swap_if(first + 1u, first + 6u, compare, projection);
            iter_swap_if(first, first + 5u, compare, projection);
            iter_swap_if(first + 1u, first + 4u, compare, projection);
            iter_swap_if(first + 6u, first + 9u, compare, projection);
            iter_swap_if(first, first + 3u, compare, projection);
            iter_swap_if(first + 5u, first + 8u, compare, projection);
            iter_swap_if(first, first + 2u, compare, projection);
            iter_swap_if(first + 3u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 9u, compare, projection);
            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 7u, compare, projection);
            iter_swap_if(first + 8u, first + 9u, compare, projection);
            iter_swap_if(first + 1u, first + 2u, compare, projection);
            iter_swap_if(first + 4u, first + 6u, compare, projection);
            iter_swap_if(first + 7u, first + 8u, compare, projection);
            iter_swap_if(first + 3u, first + 5u, compare, projection);
            iter_swap_if(first + 2u, first + 5u, compare, projection);
            iter_swap_if(first + 6u, first + 8u, compare, projection);
            iter_swap_if(first + 1u, first + 3u, compare, projection);
            iter_swap_if(first + 4u, first + 7u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);
            iter_swap_if(first + 6u, first + 7u, compare, projection);
            iter_swap_if(first + 3u, first + 4u, compare, projection);
            iter_swap_if(first + 5u, first + 6u, compare, projection);
            iter_swap_if(first + 4u, first + 5u, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 29>
        {
            return {{
                {0, 8}, {1, 9}, {2, 7}, {3, 5}, {4, 6},
                {0, 2}, {1, 4}, {5, 8}, {7, 9},
                {0, 3}, {2, 4}, {5, 7}, {6, 9},
                {0, 1}, {3, 6}, {8, 9},
                {1, 5}, {2, 3}, {4, 8}, {6, 7},
                {1, 2}, {3, 5}, {4, 6}, {7, 8},
                {2, 3}, {4, 5}, {6, 7},
                {3, 4}, {5, 6},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT10_H_
