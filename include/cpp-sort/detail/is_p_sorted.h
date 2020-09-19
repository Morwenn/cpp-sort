/*
 * Copyright (c) 2016-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_IS_P_SORTED_H_
#define CPPSORT_DETAIL_IS_P_SORTED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/as_function.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto is_p_sorted(RandomAccessIterator first, RandomAccessIterator last,
                     difference_type_t<RandomAccessIterator> p,
                     Compare compare, Projection projection)
        -> bool
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (auto it1 = first + p ; it1 != last ; ++it1) {
            auto&& value = proj(*it1);
            for (auto it2 = first ; it2 != it1 - p ; ++it2) {
                if (comp(value, proj(*it2))) {
                    return false;
                }
            }
        }
        return true;
    }
}}

#endif // CPPSORT_DETAIL_IS_P_SORTED_H_
