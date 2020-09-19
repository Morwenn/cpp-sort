/*
 * Copyright (c) 2015-2016 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT10_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT10_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../front_insert.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_comparisons_sorter_impl<10u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            low_comparisons_sorter<9u>{}(first+1u, first+10u, compare, projection);
            front_insert<10u>(std::move(first), std::move(compare), std::move(projection));
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT10_H_
