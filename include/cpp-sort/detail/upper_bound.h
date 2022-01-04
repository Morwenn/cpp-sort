/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_UPPER_BOUND_H_
#define CPPSORT_DETAIL_UPPER_BOUND_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/comparators/not_fn.h>
#include "iterator_traits.h"
#include "lower_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto upper_bound_n(ForwardIterator first, difference_type_t<ForwardIterator> size,
                       T&& value, Compare compare, Projection projection)
        -> ForwardIterator
    {
        return lower_bound_n(
            first, size,
            std::forward<T>(value),
            cppsort::not_fn(cppsort::flip(std::move(compare))),
            std::move(projection)
        );
    }

    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto upper_bound(ForwardIterator first, ForwardIterator last, T&& value,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        return lower_bound_n(
            first, std::distance(first, last),
            std::forward<T>(value),
            cppsort::not_fn(cppsort::flip(std::move(compare))),
            std::move(projection)
        );
    }
}}

#endif // CPPSORT_DETAIL_UPPER_BOUND_H_
