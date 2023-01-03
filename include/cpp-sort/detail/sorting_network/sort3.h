/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT3_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT3_H_

namespace cppsort
{
    namespace detail
    {
        template<>
        struct sorting_network_sorter_impl<3>
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
                iter_swap_if(first + 1, first + 2, compare, projection);
                iter_swap_if(first, first + 1, compare, projection);
            }

            template<typename DifferenceType=std::ptrdiff_t>
            CPPSORT_ATTRIBUTE_NODISCARD
            static constexpr auto index_pairs()
                -> std::array<utility::index_pair<DifferenceType>, 3>
            {
                return {{
                    {0, 1},
                    {1, 2},
                    {0, 1},
                }};
            }
        };
    }

    template<>
    struct sorter_traits<sorting_network_sorter<3>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::true_type;
    };
}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT3_H_
