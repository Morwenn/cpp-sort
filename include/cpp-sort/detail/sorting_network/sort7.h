/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT7_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT7_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<7>
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
            iter_swap_if(first, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first + 3, first + 6, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 4, first + 6, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 5, first + 6, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 16>
        {
            return {{
                {0, 6}, {2, 3}, {4, 5},
                {0, 2}, {1, 4}, {3, 6},
                {0, 1}, {2, 5}, {3, 4},
                {1, 2}, {4, 6},
                {2, 3}, {4, 5},
                {1, 2}, {3, 4}, {5, 6},
            }};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT7_H_
