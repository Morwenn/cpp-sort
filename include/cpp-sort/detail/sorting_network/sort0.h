/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_SORTING_NETWORK_SORT0_H_
#define CPPSORT_DETAIL_SORTING_NETWORK_SORT0_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct sorting_network_sorter_impl<0u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator,
                        Compare={}, Projection={}) const
            -> void
        {}

        template<typename DifferenceType=std::ptrdiff_t>
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 0>
        {
            return {};
        }
    };
}}

#endif // CPPSORT_DETAIL_SORTING_NETWORK_SORT0_H_
