/*
 * Copyright (c) 2015-2021 Morwenn
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
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
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
        using difference_type = difference_type_t<RandomAccessIterator>;

        // left-child of start is at 2 * start + 1
        // right-child of start is at 2 * start + 2
        difference_type child = start - first;

        if (len < 2 || (len - 2) / 2 < child)
            return;

        child = 2 * child + 1;
        RandomAccessIterator child_i = first + child;

        if ((child + 1) < len && comp(proj(*child_i), proj(*(child_i + 1)))) {
            // right-child exists and is greater than left-child
            ++child_i;
            ++child;
        }

        // check if we are in heap-order
        if (comp(proj(*child_i), proj(*start)))
            // we are, start is larger than it's largest child
            return;

        auto top = iter_move(start);
        do
        {
            // we are not in heap-order, swap the parent with it's largest child
            *start = iter_move(child_i);
            start = child_i;

            if ((len - 2) / 2 < child)
                break;

            // recompute the child based off of the updated parent
            child = 2 * child + 1;
            child_i = first + child;

            if ((child + 1) < len && comp(proj(*child_i), proj(*(child_i + 1)))) {
                // right-child exists and is greater than left-child
                ++child_i;
                ++child;
            }

            // check if we are in heap-order
        } while (!comp(proj(*child_i), proj(top)));
        *start = std::move(top);
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
                sift_down<Compare>(first, last, compare, projection, n, first + start);
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

    template<typename Compare, typename RandomAccessIterator, typename Projection>
    auto pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection,
                  difference_type_t<RandomAccessIterator> len)
        -> void
    {
        if (len > 1) {
            using utility::iter_swap;
            iter_swap(first, --last);
            sift_down<Compare>(first, last, compare, std::move(projection), len - 1, first);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto sort_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        for (difference_type n = last - first; n > 1; --last, (void) --n) {
            pop_heap<Compare>(first, last, compare, projection, n);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto heapsort(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection)
        -> void
    {
        make_heap(first, last, compare, projection);
        sort_heap(std::move(first), std::move(last),
                  std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_HEAPSORT_H_
