/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_LOWER_BOUND_H_
#define CPPSORT_DETAIL_LOWER_BOUND_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto lower_bound_n(ForwardIterator first, difference_type_t<ForwardIterator> size,
                       T&& value, Compare compare, Projection projection)
        -> ForwardIterator
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (size > 0) {
            ForwardIterator it = first;
            std::advance(it, half(size));
            if (comp(proj(*it), value)) {
                first = ++it;
                size -= half(size) + 1;
            } else {
                size = half(size);
            }
        }
        return first;
    }

    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto lower_bound(ForwardIterator first, ForwardIterator last, T&& value,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        return lower_bound_n(first, std::distance(first, last),
                             std::forward<T>(value),
                             std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_LOWER_BOUND_H_
