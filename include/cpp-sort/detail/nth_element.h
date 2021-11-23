/*
 * Copyright (c) 2018-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_NTH_ELEMENT_H_
#define CPPSORT_DETAIL_NTH_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include "adaptive_quickselect.h"
#include "introselect.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // nth_element for forward iterators with introselect

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto nth_element(std::forward_iterator_tag,
                     ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        return introselect(first, last, nth_pos, size,
                           std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // nth_element for random-access iterators with Andrei
    // Alexandrescu's adaptive quickselect

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto nth_element(std::random_access_iterator_tag,
                     RandomAccessIterator first, RandomAccessIterator last,
                     difference_type_t<RandomAccessIterator> nth_pos,
                     difference_type_t<RandomAccessIterator>, // unused
                     Compare compare, Projection projection)
        -> RandomAccessIterator
    {
        return median_of_ninthers_select(first, first + nth_pos, last,
                                         std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Generic nth_element overload, slightly modified compared
    // to the standard library one to avoid recomputing sizes
    // over and over again, which might be too expensive for
    // forward and bidirectional iterators - also returns the
    // nth iterator

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto nth_element(ForwardIterator first, ForwardIterator last,
                     difference_type_t<ForwardIterator> nth_pos,
                     difference_type_t<ForwardIterator> size,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        using category = iterator_category_t<ForwardIterator>;
        return detail::nth_element(category{}, first, last, nth_pos, size,
                                   std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_NTH_ELEMENT_H_
