/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_FRONT_INSERTER2_H_
#define CPPSORT_DETAIL_FRONT_INSERTER2_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/as_function.h>
#include "../rotate_left.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct front_inserter_n<2u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare,
            typename Projection
        >
        auto operator()(RandomAccessIterator first, Compare compare, Projection projection) const
            -> void
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (comp(proj(first[1u]), proj(first[0u]))) {
                rotate_left<2u>(first);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_FRONT_INSERTER0_H_
