/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT2_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT2_H_

namespace cppsort
{
    namespace detail
    {
        template<>
        struct sorting_network_sorter_impl<2>
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
            }

            template<typename DifferenceType=std::ptrdiff_t>
            [[nodiscard]] static constexpr auto index_pairs() noexcept
                -> std::array<utility::index_pair<DifferenceType>, 1>
            {
                return {{
                    {0, 1},
                }};
            }
        };
    }

    template<>
    struct sorter_traits<sorting_network_sorter<2>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::true_type;
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT2_H_
