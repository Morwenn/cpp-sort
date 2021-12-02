/*
 * Copyright (c) 2016-2021 Morwenn
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
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto is_p_sorted(ForwardIterator first, ForwardIterator last, ForwardIterator pth,
                     Compare compare, Projection projection)
        -> bool
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // pth is the iterator such as pth - first == p

        auto max_it = first;
        for (auto it1 = std::next(pth); it1 != last; ++it1) {
            if (comp(proj(*max_it), proj(*first))) {
                max_it = first;
            }
            if (comp(proj(*it1), proj(*max_it))) {
                return false;
            }
            ++first;
        }
        return true;
    }
}}

#endif // CPPSORT_DETAIL_IS_P_SORTED_H_
