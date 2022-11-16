/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT2_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT2_H_

namespace cppsort::detail
{
    template<>
    struct low_comparisons_sorter_impl<2u>
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
            iter_swap_if(first, first + 1u,
                         std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT2_H_
