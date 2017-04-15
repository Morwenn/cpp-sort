// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// //  Modified in 2015-2017 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_HEAP_OPERATIONS_H_
#define CPPSORT_DETAIL_HEAP_OPERATIONS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<class Compare, class RandomAccessIterator, class Projection>
    auto sift_down(RandomAccessIterator first, RandomAccessIterator,
                   Compare comp, Projection projection,
                   difference_type_t<RandomAccessIterator> len,
                   RandomAccessIterator start)
        -> void
    {
        using utility::iter_move;
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

    template<class RandomAccessIterator, class Compare, class Projection>
    auto make_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare comp, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        difference_type n = std::distance(first, last);
        if (n > 1)
        {
            // start from the first parent, there is no need to consider children
            for (difference_type start = (n - 2) / 2; start >= 0; --start)
            {
                sift_down<Compare>(first, last, comp, projection, n, first + start);
            }
        }
    }

    template<class Compare, class RandomAccessIterator, class Projection>
    auto pop_heap(RandomAccessIterator first, RandomAccessIterator last,
                  Compare comp, Projection projection,
                  difference_type_t<RandomAccessIterator> len)
        -> void
    {
        if (len > 1)
        {
            using utility::iter_swap;
            iter_swap(first, --last);
            sift_down<Compare>(first, last, comp, std::move(projection), len - 1, first);
        }
    }

    template<class RandomAccessIterator, class Compare, class Projection>
    auto sort_heap(RandomAccessIterator first, RandomAccessIterator last,
                   Compare comp, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<RandomAccessIterator>;
        for (difference_type n = last - first; n > 1; --last, (void) --n) {
            pop_heap<Compare>(first, last, comp, projection, n);
        }
    }
}}

#endif // CPPSORT_DETAIL_HEAP_OPERATIONS_H_
