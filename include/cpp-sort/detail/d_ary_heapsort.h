/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
// boost heap: d-ary heap as container adaptor
//
// Copyright (C) 2010 Tim Blechmann
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef CPPSORT_DETAIL_D_ARY_HEAPSORT_H_
#define CPPSORT_DETAIL_D_ARY_HEAPSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/utility/as_function.h>
#include "min_element.h"

namespace cppsort::detail
{
    template<int D, typename RandomAccessIterator>
    constexpr auto parent_it(RandomAccessIterator first, RandomAccessIterator it)
        -> RandomAccessIterator
    {
        auto index = it - first;
        return first + (index - 1) / D;
    }

    template<int D, typename RandomAccessIterator>
    constexpr auto first_child_it(RandomAccessIterator first, RandomAccessIterator it)
        -> RandomAccessIterator
    {
        auto index = it - first;
        return first + index * D + 1;
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto top_child_it(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator it,
                                Compare compare, Projection projection)
        -> RandomAccessIterator
    {
        // invariant: it is not a leaf, so the iterator range is not empty

        auto first_child = first_child_it<D>(first, it);
        auto max_elements = last - first_child;
        auto last_child = (max_elements > D) ? first_child + D : last;

        return detail::min_element(
            first_child, last_child,
            cppsort::flip(std::move(compare)),
            std::move(projection)
        );
    }

    template<int D, typename RandomAccessIterator>
    constexpr auto not_leaf(RandomAccessIterator first, RandomAccessIterator last,
                            RandomAccessIterator it)
        -> bool
    {
        return (it - first) * (D - 1) < last - it - 1;
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto siftdown(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator it,
                            Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (not_leaf<D>(first, last, it)) {
            auto max_child = top_child_it<D>(first, last, it, compare, projection);
            if (not comp(proj(*max_child), proj(*it))) {
                mstd::iter_swap(max_child, it);
                it = max_child;
            } else {
                return;
            }
        }
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto siftup(RandomAccessIterator first, RandomAccessIterator it,
                          Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (it != first) {
            auto parent = parent_it<D>(first, it);

            if (comp(proj(*parent), proj(*it))) {
                mstd::iter_swap(parent, it);
                it = parent;
            } else {
                return;
            }
        }
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto push_d_ary_heap(RandomAccessIterator first, RandomAccessIterator last,
                                   Compare compare, Projection projection)
        -> void
    {
        siftup<D>(first, std::prev(last), std::move(compare), std::move(projection));
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto pop_d_ary_heap(RandomAccessIterator first, RandomAccessIterator last,
                                  Compare compare, Projection projection)
        -> void
    {
        mstd::iter_swap(first, --last);
        if (first == last) return;

        siftdown<D>(first, last, first, std::move(compare), std::move(projection));
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto make_d_ary_heap(RandomAccessIterator first, RandomAccessIterator last,
                                   Compare compare, Projection projection)
        -> void
    {
        if (first == last) return;

        for (auto it = std::next(first); it != last; ++it) {
            push_d_ary_heap<D>(first, it, compare, projection);
        }
        // Take the last element into consideration
        push_d_ary_heap<D>(first, last, std::move(compare), std::move(projection));
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto sort_d_ary_heap(RandomAccessIterator first, RandomAccessIterator last,
                                   Compare compare, Projection projection)
        -> void
    {
        for (auto it = last; it != first; --it) {
            pop_d_ary_heap<D>(first, it, compare, projection);
        }
    }

    template<int D, typename RandomAccessIterator, typename Compare, typename Projection>
    constexpr auto d_ary_heapsort(RandomAccessIterator first, RandomAccessIterator last,
                                  Compare compare, Projection projection)
        -> void
    {
        make_d_ary_heap<D>(first, last, compare, projection);
        sort_d_ary_heap<D>(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_D_ARY_HEAPSORT_H_
