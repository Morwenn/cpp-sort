/*
 * Copyright (c) 2015 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FRONT_INSERTER0_H_
#define CPPSORT_DETAIL_FRONT_INSERTER0_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct front_inserter_n<0u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare,
            typename Projection
        >
        auto operator()(RandomAccessIterator, Compare, Projection) const
            -> void
        {}
    };
}}

#endif // CPPSORT_DETAIL_FRONT_INSERTER0_H_
