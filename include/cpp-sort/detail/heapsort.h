/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===/

#ifndef CPPSORT_DETAIL_HEAPSORT_H_
#define CPPSORT_DETAIL_HEAPSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename Compare, typename RandomAccessIterator, typename Projection>
    auto sift_down(RandomAccessIterator first, RandomAccessIterator,
                   Compare compare, Projection projection,
                   difference_type_t<RandomAccessIterator> len,
                   RandomAccessIterator start)
        -> void
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // left-child of start is at 2 * start + 1
        // right-child of start is at 2 * start + 2
        auto child = start - first;

        if (len < 2 || (len - 2) / 2 < child) {
            return;
        }

        child = 2 * child + 1;
        auto child_i = first + child;

        if ((child + 1) < len && comp(proj(*child_i), proj(*(child_i + 1)))) {
            // right-child exists and is greater than left-child
            ++child_i;
            ++child;
        }

        // check if we are in heap-order
        if (comp(proj(*child_i), proj(*start))) {
            // we are, start is larger than it's largest child
            return;
        }

        auto top = iter_move(start);
        do {
            // we are not in heap-order, swap the parent with it's largest child
            *start = iter_move(child_i);
            start = child_i;

            if ((len - 2) / 2 < child) {
                break;
            }

            // recompute the child based off of the updated parent
            child = 2 * child + 1;
            child_i = first + child;

            if ((child + 1) < len && comp(proj(*child_i), proj(*(child_i + 1)))) {
                // right-child exists and is greater than left-child
                ++child_i;
                ++child;
            }

            // check if we are in heap-order
        } while (not comp(proj(*child_i), proj(top)));
        *start = std::move(top);
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto floyd_sift_down(RandomAccessIterator first, Compare compare, Projection projection,
                         difference_type_t<RandomAccessIterator> len)
        -> RandomAccessIterator
    {
        CPPSORT_ASSERT(len >= 2);

        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        using difference_type = difference_type_t<RandomAccessIterator>;

        auto hole = first;
        auto child_i = first;
        difference_type child = 0;

        while (true) {
            child_i += difference_type(child + 1);
            child = 2 * child + 1;

            if ((child + 1) < len && comp(proj(*child_i), proj(*std::next(child_i)))) {
                // right-child exists and is greater than left-child
                ++child_i;
                ++child;
            }

            // swap hole with its largest child
            *hole = iter_move(child_i);
            hole = child_i;

            // if hole is now a leaf, we're done
            if (child > (len - 2) / 2) {
                return hole;
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto push_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection,
                   difference_type_t<RandomAccessIterator> len)
    {
        using utility::iter_move;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (len > 1) {
            len = (len - 2) / 2;
            auto ptr = first + len;
            if (comp(proj(*ptr), proj(*--last))) {
                auto t = iter_move(last);
                auto&& proj_t = proj(t);
                do {
                    *last = iter_move(ptr);
                    last = ptr;
                    if (len == 0) {
                        break;
                    }
                    len = (len - 1) / 2;
                    ptr = first + len;
                } while (comp(proj(*ptr), proj_t));
                *last = std::move(t);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection,
                  difference_type_t<RandomAccessIterator> len)
        -> void
    {
        using utility::iter_move;

        if (len > 1) {
            auto top = iter_move(first);  // create a hole at first
            auto hole = detail::floyd_sift_down(first, compare, projection, len);
            if (hole == --last) {
                *hole = std::move(top);
            } else {
                *hole = iter_move(last);
                ++hole;
                *last = std::move(top);
                detail::push_heap(first, hole, compare, projection, hole - first);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto make_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        difference_type n = last - first;
        if (n > 1) {
            // start from the first parent, there is no need to consider children
            for (difference_type start = (n - 2) / 2; start >= 0; --start) {
                detail::sift_down(first, last, compare, projection, n, first + start);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto sort_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection)
        -> void
    {
        for (auto n = last - first; n > 1; --last, (void) --n) {
            detail::pop_heap(first, last, compare, projection, n);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto heapsort(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection)
        -> void
    {
        detail::make_heap(first, last, compare, projection);
        detail::sort_heap(std::move(first), std::move(last),
                          std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_HEAPSORT_H_
