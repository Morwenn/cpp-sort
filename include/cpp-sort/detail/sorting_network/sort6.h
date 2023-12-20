/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT6_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT6_H_

namespace cppsort::detail
{
    template<>
    struct sorting_network_sorter_impl<6>
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
            iter_swap_if(first, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 4, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
            iter_swap_if(first, first + 3, compare, projection);
            iter_swap_if(first + 2, first + 5, compare, projection);
            iter_swap_if(first, first + 1, compare, projection);
            iter_swap_if(first + 2, first + 3, compare, projection);
            iter_swap_if(first + 4, first + 5, compare, projection);
            iter_swap_if(first + 1, first + 2, compare, projection);
            iter_swap_if(first + 3, first + 4, compare, projection);
        }

        template<typename DifferenceType=std::ptrdiff_t>
        [[nodiscard]] static constexpr auto index_pairs() noexcept
            -> std::array<utility::index_pair<DifferenceType>, 12>
        {
            return {{
                {0, 5}, {1, 3}, {2, 4},
                {1, 2}, {3, 4},
                {0, 3}, {2, 5},
                {0, 1}, {2, 3}, {4, 5},
                {1, 2}, {3, 4},
            }};
        }
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT6_H_
