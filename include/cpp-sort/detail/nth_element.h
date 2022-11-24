/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_NTH_ELEMENT_H_
#define CPPSORT_DETAIL_NTH_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "adaptive_quickselect.h"
#include "introselect.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // nth_element implementation, slightly modified compared
    // to the standard library one to avoid recomputing sizes
    // over and over again, which might be too expensive for
    // forward and bidirectional iterators - also returns the
    // nth iterator

    ////////////////////////////////////////////////////////////
    // nth_element for forward iterators with introselect

    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto nth_element(Iterator first, Iterator last,
                               mstd::iter_difference_t<Iterator> nth_pos,
                               mstd::iter_difference_t<Iterator> size,
                               Compare compare, Projection projection)
        -> Iterator
    {
        return introselect(
            first, last, nth_pos, size,
            std::move(compare), std::move(projection)
        );
    }

    ////////////////////////////////////////////////////////////
    // nth_element for random-access iterators with Andrei
    // Alexandrescu's adaptive quickselect

    template<
        mstd::random_access_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto nth_element(Iterator first, Iterator last,
                               mstd::iter_difference_t<Iterator> nth_pos,
                               mstd::iter_difference_t<Iterator>, // unused
                               Compare compare, Projection projection)
        -> Iterator
    {
        return median_of_ninthers_select(
            first, first + nth_pos, last,
            std::move(compare), std::move(projection)
        );
    }
}

#endif // CPPSORT_DETAIL_NTH_ELEMENT_H_
