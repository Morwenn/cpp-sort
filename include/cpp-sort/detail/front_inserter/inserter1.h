/*
 * Copyright (c) 2015 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FRONT_INSERTER1_H_
#define CPPSORT_DETAIL_FRONT_INSERTER1_H_

namespace cppsort
{
namespace detail
{
    template<>
    struct front_inserter_n<1u>
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

#endif // CPPSORT_DETAIL_FRONT_INSERTER1_H_
