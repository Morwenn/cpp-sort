/*
 * Copyright (c) 2015-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ITER_SORT3_H_
#define CPPSORT_DETAIL_ITER_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include "swap_if.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator, typename Compare, typename Projection>
    auto iter_sort3(Iterator a, Iterator b, Iterator c,
                    Compare compare, Projection projection)
        -> Iterator
    {
        iter_swap_if(b, c, compare, projection);
        iter_swap_if(a, c, compare, projection);
        iter_swap_if(a, b, std::move(compare), std::move(projection));
        return b; // Return median of 3
    }
}}

#endif // CPPSORT_DETAIL_ITER_SORT3_H_
