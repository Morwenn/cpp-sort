/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_QUICKSORT_H_
#define CPPSORT_DETAIL_QUICKSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "bitops.h"
#include "bubble_sort.h"
#include "insertion_sort.h"
#include "introselect.h"
#include "iterator_traits.h"
#include "partition.h"

namespace cppsort
{
namespace detail
{
    // Function that dispatches the sort to another algorithm
    // when the collection is too small for quicksort to be
    // worth it
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto quicksort_fallback(ForwardIterator first, ForwardIterator,
                            difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection,
                            std::forward_iterator_tag)
        -> bool
    {
        if (size < 10) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return true;
        }
        return false;
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto quicksort_fallback(BidirectionalIterator first, BidirectionalIterator last,
                            difference_type_t<BidirectionalIterator> size,
                            Compare compare, Projection projection,
                            std::bidirectional_iterator_tag)
        -> bool
    {
        if (size < 42) {
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return true;
        }
        return false;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto quicksort(ForwardIterator first, ForwardIterator last,
                   difference_type_t<ForwardIterator> size, int bad_allowed,
                   Compare compare, Projection projection)
        -> void
    {
        using difference_type = difference_type_t<ForwardIterator>;
        using utility::iter_swap;

        // If the collection is small enough, fall back to
        // another sorting algorithm
        using category = iterator_category_t<ForwardIterator>;
        bool sorted = quicksort_fallback(first, last, size, compare, projection, category{});
        if (sorted) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Choose pivot as either median of 9 or median of medians
        auto temp = pick_pivot(first, last, size, bad_allowed, compare, projection);
        auto median_it = temp.first;
        auto last_1 = temp.second;

        // Put the pivot at position std::prev(last) and partition
        iter_swap(median_it, last_1);
        auto&& pivot1 = proj(*last_1);
        auto middle1 = detail::partition(
            first, last_1,
            [&](auto&& elem) { return comp(proj(elem), pivot1); }
        );

        // Put the pivot in its final position and partition
        iter_swap(middle1, last_1);
        auto&& pivot2 = proj(*middle1);
        auto middle2 = detail::partition(
            std::next(middle1), last,
            [&](auto&& elem) { return not comp(pivot2, proj(elem)); }
        );

        // Recursive call: heuristic trick here: in real world cases,
        // the middle partition is more likely to be smaller than the
        // right one, so computing its size should generally be cheaper
        auto size_left = std::distance(first, middle1);
        auto size_middle = std::distance(middle1, middle2);
        difference_type size_right = size - size_left - size_middle;

        // Recurse in the smallest partition first to limit the call
        // stack overhead
        if (size_left > size_right) {
            std::swap(first, middle2);
            std::swap(middle1, last);
            std::swap(size_left, size_right);
        }

        --bad_allowed;
        quicksort(first, middle1, size_left, bad_allowed, compare, projection);
        quicksort(middle2, last, size_right, bad_allowed,
                  std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto quicksort(ForwardIterator first, ForwardIterator last,
                   difference_type_t<ForwardIterator> size,
                   Compare compare, Projection projection)
        -> void
    {
        int bad_allowed = 2 * detail::log2(size);  // Usual introsort recursion limit
        quicksort(first, last, size, bad_allowed, std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_QUICKSORT_H_
