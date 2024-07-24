/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_INTROSELECT_H_
#define CPPSORT_DETAIL_INTROSELECT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "config.h"
#include "insertion_sort.h"
#include "iter_sort3.h"
#include "partition.h"
#include "selection_sort.h"
#include "swap_if.h"

namespace cppsort::detail
{
    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto small_sort(Iterator first, Iterator last,
                              Compare compare, Projection projection)
        -> void
    {
        selection_sort(std::move(first), std::move(last),
                       std::move(compare), std::move(projection));
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    constexpr auto small_sort(Iterator first, Iterator last,
                              Compare compare, Projection projection)
        -> void
    {
        insertion_sort(std::move(first), std::move(last),
                       std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto iter_median_5(ForwardIterator it1, ForwardIterator it2, ForwardIterator it3,
                                 ForwardIterator it4, ForwardIterator it5,
                                 Compare compare, Projection projection)
        -> ForwardIterator
    {
        // Median of 5, adapted from https://stackoverflow.com/a/481398/1364752

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        iter_swap_if(it1, it2, compare, projection);
        iter_swap_if(it3, it4, compare, projection);

        if (comp(proj(*it1), proj(*it3))) {
            mstd::iter_swap(it1, it5);
            iter_swap_if(it1, it2, compare, projection);
        } else {
            mstd::iter_swap(it3, it5);
            iter_swap_if(it3, it4, compare, projection);
        }

        if (comp(proj(*it1), proj(*it3))) {
            if (comp(proj(*it2), proj(*it3))) {
                return it3;
            }
            return it2;
        } else {
            if (comp(proj(*it4), proj(*it1))) {
                return it1;
            }
            return it4;
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto iter_median_rest(ForwardIterator first, mstd::iter_difference_t<ForwardIterator> size,
                                    Compare compare, Projection projection)
        -> ForwardIterator
    {
        switch (size) {
            case 0:
            case 1:
            case 2:
                return first;
            case 3: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                iter_swap_if(it1, it2, compare, projection);
                iter_swap_if(it2, it3, compare, projection);
                iter_swap_if(it1, it2, compare, projection);
                return it2;
            }
            case 4: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                auto it4 = ++first;
                iter_swap_if(it1, it2, compare, projection);
                iter_swap_if(it3, it4, compare, projection);
                iter_swap_if(it1, it3, compare, projection);
                iter_swap_if(it2, it4, compare, projection);
                iter_swap_if(it2, it3, compare, projection);
                return it2;
            }
            case 5: {
                auto it1 = first;
                auto it2 = ++first;
                auto it3 = ++first;
                auto it4 = ++first;
                auto it5 = ++first;
                return iter_median_5(it1, it2, it3, it4, it5,
                                     std::move(compare), std::move(projection));
            }
            default:
                CPPSORT_UNREACHABLE;
        }
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto introselect(ForwardIterator first, ForwardIterator last,
                               mstd::iter_difference_t<ForwardIterator> nth_pos,
                               mstd::iter_difference_t<ForwardIterator> size,
                               Compare compare, Projection projection)
        -> ForwardIterator;

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto median_of_medians(ForwardIterator first, ForwardIterator last,
                                     mstd::iter_difference_t<ForwardIterator> size,
                                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        if (size <= 5) {
            return iter_median_rest(first, size, std::move(compare), std::move(projection));
        }

        // Iterator over the collection
        auto it = first;
        // Points to the next value to replace by a median-of-5
        auto medians_it = first;

        // We handle first the biggest part that can be rounded to a power
        // of 5, then we handle the rest
        auto rounded_size = (size / 5) * 5;

        // Handle elements 5 by 5
        for (mstd::iter_difference_t<ForwardIterator> i = 0 ; i < rounded_size / 5 ; ++i) {
            auto it1 = it;
            auto it2 = ++it;
            auto it3 = ++it;
            auto it4 = ++it;
            auto it5 = ++it;

            auto median = iter_median_5(it1, it2, it3, it4, it5, compare, projection);
            mstd::iter_swap(medians_it, median);
            ++medians_it;
            ++it;
        }

        // Handle remaining elements
        if (rounded_size != size) {
            auto last_median = iter_median_rest(it, size - rounded_size, compare, projection);
            mstd::iter_swap(last_median, medians_it);
            ++medians_it;
        }

        // Reset variables for the next iteration
        last = medians_it;
        size = rounded_size == size ? size / 5 : size / 5 + 1;

        // Mutual recursion with introselect
        return introselect(first, last, size / 2, size,
                           std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Get iterator to last element

    template<mstd::forward_iterator Iterator>
    constexpr auto last_it(Iterator first, Iterator, mstd::iter_difference_t<Iterator> size)
        -> Iterator
    {
        return mstd::next(first, size - 1);
    }

    template<mstd::bidirectional_iterator Iterator>
    constexpr auto last_it(Iterator, Iterator last, mstd::iter_difference_t<Iterator>)
        -> Iterator
    {
        return mstd::prev(last);
    }

    ////////////////////////////////////////////////////////////
    // Pick a pivot for quicksort and quickselect

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto pick_pivot(ForwardIterator first, ForwardIterator last,
                              mstd::iter_difference_t<ForwardIterator> size, int bad_allowed,
                              Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        if (bad_allowed > 0) {
            auto it1 = mstd::next(first, size / 8);
            auto it2 = mstd::next(it1, size / 8);
            auto it3 = mstd::next(it2, size / 8);
            auto middle = mstd::next(it3, size/2 - 3*(size/8));
            auto it4 = mstd::next(middle, size / 8);
            auto it5 = mstd::next(it4, size / 8);
            auto it6 = mstd::next(it5, size / 8);
            auto last_1 = last_it(it6, last, size - size/2 - 3*(size/8));

            iter_sort3(first, it1, it2, compare, projection);
            iter_sort3(it3, middle, it4, compare, projection);
            iter_sort3(it5, it6, last_1, compare, projection);
            auto median_it = iter_sort3(it1, middle, it6, std::move(compare), std::move(projection));
            return std::make_pair(median_it, last_1);
        } else {
            auto last_1 = last_it(first, last, size);
            auto median_it = median_of_medians(first, last, size, compare, projection);
            return std::make_pair(median_it, last_1);
        }
    }

    ////////////////////////////////////////////////////////////
    // Introselect

    template<typename ForwardIterator, typename Compare, typename Projection>
    constexpr auto introselect(ForwardIterator first, ForwardIterator last,
                               mstd::iter_difference_t<ForwardIterator> nth_pos,
                               mstd::iter_difference_t<ForwardIterator> size,
                               Compare compare, Projection projection)
        -> ForwardIterator
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        int bad_allowed = detail::log2(size);

        while (size > 32) {
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
            auto size_right = size - size_left - size_middle;

            // We're done if the nth element is in the middle partition
            if (nth_pos < size_left) {
                last = middle1;
                size = size_left;
            } else if (nth_pos > size_left + size_middle) {
                first = middle2;
                nth_pos -= size_left + size_middle;
                size = size_right;
            } else {
                // Return an iterator to the nth element
                return mstd::next(middle1, nth_pos - size_left);
            }
            --bad_allowed;
        }
        // Fallback when the collection is small enough
        small_sort(first, last, std::move(compare), std::move(projection));
        return mstd::next(first, nth_pos);
    }
}

#endif // CPPSORT_DETAIL_INTROSELECT_H_
