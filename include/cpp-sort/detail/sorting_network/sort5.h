/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_

namespace cppsort::detail
{
    template<>
    struct sorting_network_sorter_impl<5>
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
            iter_swap_if(first, first + 3, compare, projection);
            iter_swap_if(first + 1, first + 4, compare, projection);
            iter_swap_if(first, first + 2, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        [[nodiscard]] static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 9>
        {
            return {{
                {0, 3}, {1, 4},
                {0, 2}, {1, 3},
                {0, 1}, {2, 4},
                {1, 2}, {3, 4},
                {2, 3},
            }};
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT5_H_
