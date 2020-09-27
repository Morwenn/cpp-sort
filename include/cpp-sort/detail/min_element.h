/*
 * Copyright (c) 2015-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MIN_ELEMENT_H_
#define CPPSORT_DETAIL_MIN_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "config.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto unchecked_min_element(ForwardIterator first, ForwardIterator last,
                               Compare compare, Projection projection)
        -> ForwardIterator
    {
        // Same algorithm as min_element, but assumes that the
        // the input range is never empty
        CPPSORT_ASSUME(first != last);

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        auto min = first;
        while (++first != last) {
            if (comp(proj(*first), proj(*min))) {
                min = first;
            }
        }
        return min;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto min_element(ForwardIterator first, ForwardIterator last,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        if (first == last) return last;
        return unchecked_min_element(std::move(first), std::move(last),
                                     std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_MIN_ELEMENT_H_
