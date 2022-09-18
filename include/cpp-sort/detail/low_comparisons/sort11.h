/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT11_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT11_H_

namespace cppsort::detail
{
    template<>
    struct low_comparisons_sorter_impl<11u>
    {
        template<
            mstd::random_access_iterator Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = mstd::enable_if_t<is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator first, Iterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            low_comparisons_sorter<10u>{}(first+1u, first+11u, compare, projection);
            front_insert<11u>(std::move(first), std::move(compare), std::move(projection));
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT11_H_
