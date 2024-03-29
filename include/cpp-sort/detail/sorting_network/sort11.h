/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<11>
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
            iter_swap_if(first, first + 9, compare, projection);
            iter_swap_if(first + 1, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 8, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 3, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 10, compare, projection);
            iter_swap_if(first + 6, first + 9, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 4, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 10, compare, projection);
            iter_swap_if(first, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 7, compare, projection);
            iter_swap_if(first + 5, first + 9, compare, projection);
            iter_swap_if(first + 6, first + 8, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 6, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 9, first + 10, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first + 5, first + 7, compare, projection);
            iter_swap_if(first + 8, first + 9, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
            iter_swap_if(first + 7, first + 8, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 6, first + 7, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 35>
        {
            return {{
                {0, 9}, {1, 6}, {2, 4}, {3, 7}, {5, 8},
                {0, 1}, {3, 5}, {4, 10}, {6, 9}, {7, 8},
                {1, 3}, {2, 5}, {4, 7}, {8, 10},
                {0, 4}, {1, 2}, {3, 7}, {5, 9}, {6, 8},
                {0, 1}, {2, 6}, {4, 5}, {7, 8}, {9, 10},
                {2, 4}, {3, 6}, {5, 7}, {8, 9},
                {1, 2}, {3, 4}, {5, 6}, {7, 8},
                {2, 3}, {4, 5}, {6, 7},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT11_H_
