/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_QUICKSORT_H_
#define CPPSORT_DETAIL_QUICKSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <iterator>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "bubble_sort.h"
#include "insertion_sort.h"
#include "introselect.h"
#include "partition.h"

namespace cppsort::detail
{
    // Function that dispatches the sort to another algorithm
    // when the collection is too small for quicksort to be
    // worth it
    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto quicksort_fallback(Iterator first, Iterator,
                                      mstd::iter_difference_t<Iterator> size,
                                      Compare compare, Projection projection)
        -> bool
    {
        if (size < 10) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return true;
        }
        return false;
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto quicksort_fallback(Iterator first, Iterator last,
                                      mstd::iter_difference_t<Iterator> size,
                                      Compare compare, Projection projection)
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
    constexpr auto quicksort(ForwardIterator first, ForwardIterator last,
                             mstd::iter_difference_t<ForwardIterator> size, int bad_allowed,
                             Compare compare, Projection projection)
        -> void
    {
        using difference_type = mstd::iter_difference_t<ForwardIterator>;

        // If the collection is small enough, fall back to
        // another sorting algorithm
        bool sorted = quicksort_fallback(first, last, size, compare, projection);
        if (sorted) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Choose pivot as either median of 9 or median of medians
        auto temp = pick_pivot(first, last, size, bad_allowed, compare, projection);
        auto median_it = temp.first;
        auto last_1 = temp.second;

        // Put the pivot at position mstd::prev(last) and partition
        mstd::iter_swap(median_it, last_1);
        auto&& pivot1 = proj(*last_1);
        auto middle1 = detail::partition(
            first, last_1,
            [&](auto&& elem) { return comp(proj(elem), pivot1); }
        );

        // Put the pivot in its final position and partition
        mstd::iter_swap(middle1, last_1);
        auto&& pivot2 = proj(*middle1);
        auto middle2 = detail::partition(
            mstd::next(middle1), last,
            [&](auto&& elem) { return not comp(pivot2, proj(elem)); }
        );

        // Recursive call: heuristic trick here: in real world cases,
        // the middle partition is more likely to be smaller than the
        // right one, so computing its size should generally be cheaper
        auto size_left = mstd::distance(first, middle1);
        auto size_middle = mstd::distance(middle1, middle2);
        difference_type size_right = size - size_left - size_middle;

        // Recurse in the smallest partition first to limit the call
        // stack overhead
        if (size_left > size_right) {
            std::ranges::swap(first, middle2);
            std::ranges::swap(middle1, last);
            std::ranges::swap(size_left, size_right);
        }

        --bad_allowed;
        quicksort(first, middle1, size_left, bad_allowed, compare, projection);
        quicksort(middle2, last, size_right, bad_allowed,
                  std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto quicksort(ForwardIterator first, ForwardIterator last,
                             mstd::iter_difference_t<ForwardIterator> size,
                             Compare compare, Projection projection)
        -> void
    {
        int bad_allowed = 2 * detail::log2(size);  // Usual introsort recursion limit
        quicksort(first, last, size, bad_allowed, std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_QUICKSORT_H_
