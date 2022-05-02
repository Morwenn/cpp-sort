/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_QUICK_MERGE_SORT_H_
#define CPPSORT_DETAIL_QUICK_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "iterator_traits.h"
#include "nth_element.h"
#include "quicksort.h"
#include "sized_iterator.h"
#include "swap_ranges.h"

namespace cppsort
{
namespace detail
{
    constexpr int qmsort_limit = 32;

    template<typename ForwardIterator1, typename ForwardIterator2, typename OutputIterator,
             typename Size, typename Compare, typename Projection>
    auto internal_half_inplace_merge(ForwardIterator1 first1, ForwardIterator1 last1,
                                     ForwardIterator2 first2, ForwardIterator2 last2,
                                     OutputIterator result, Size size_left,
                                     Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (; size_left != 0 ; --size_left) {
            CPPSORT_ASSUME(first1 != last1);
            CPPSORT_ASSUME(first2 != last2);
            if (comp(proj(*first2), proj(*first1))) {
                iter_swap(result, first2);
                ++first2;
            } else {
                iter_swap(result, first1);
                ++first1;
            }
            ++result;
        }

        for (; first1 != last1; ++result) {
            if (first2 == last2) {
                detail::swap_ranges_inner(first1, last1, result);
                return;
            }

            if (comp(proj(*first2), proj(*first1))) {
                iter_swap(result, first2);
                ++first2;
            } else {
                iter_swap(result, first1);
                ++first1;
            }
        }
        // first2 through last2 are already in the right place
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto internal_buffered_inplace_merge(ForwardIterator first, ForwardIterator middle,
                                         ForwardIterator last,
                                         difference_type_t<ForwardIterator> size_left,
                                         ForwardIterator buffer,
                                         Compare compare, Projection projection)
        -> void
    {
        auto buffer_end = detail::swap_ranges_inner(first, middle, buffer);
        internal_half_inplace_merge(buffer, buffer_end, middle, last, first, size_left,
                                    std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto internal_mergesort(ForwardIterator first, ForwardIterator last,
                            difference_type_t<ForwardIterator> size,
                            ForwardIterator buffer,
                            Compare compare, Projection projection)
        -> void
    {
        if (size <= qmsort_limit) {
            small_sort(first, last, std::move(compare), std::move(projection));
            return;
        }

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Ensure left partition is smaller: in a typical inplace_merge implementation,
        // the smallest partition is moved to the buffer, but we need the move the left
        // one to be able to safely store the result from the beginning of the collection;
        // it can also be done from the end but requires bidirectional iterators, so all
        // in all, ensuring that the left partition is smaller allows to use the algorithm
        // with forward iterators
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively mergesort the to partitions
        internal_mergesort(first, middle, size_left, buffer, compare, projection);
        internal_mergesort(middle, last, size - size_left, buffer, compare, projection);

        // Reduce left partition even more if possible
        auto&& mid_value = proj(*middle);
        while (first != middle && not comp(mid_value, proj(*first))) {
            ++first;
            --size_left;
        }
        if (first == middle) return;

        internal_buffered_inplace_merge(first, middle, last, size_left, buffer,
                                        std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto quick_merge_sort(ForwardIterator first, ForwardIterator last,
                          difference_type_t<ForwardIterator> size,
                          Compare compare, Projection projection)
        -> void
    {
        // This flavour of QuickMergesort splits the collection in [2/3, 1/3]
        // partitions where the right partition is used as an internal buffer
        // to apply mergesort to the left partition, then QuickMergesort is
        // recursively applied to the smaller right partition

        while (size > qmsort_limit) {
            // This represents both the size of the left partition
            // and the position of the pivot
            auto size_left = 2 * (size / 3) - 2;
            auto pivot = detail::nth_element(first, last, size_left, size, compare, projection);
            internal_mergesort(first, pivot, size_left, pivot, compare, projection);

            if (std::is_base_of<std::random_access_iterator_tag, iterator_category_t<ForwardIterator>>::value) {
                // Avoid weird codegen bug with MinGW-w64 (see GitHub issue #151)
                std::advance(first, size_left);
            } else {
                first = pivot;
            }
            size -= size_left;
        }
        small_sort(first, last, std::move(compare), std::move(projection));
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto quick_merge_sort(sized_iterator<ForwardIterator> first, sized_iterator<ForwardIterator> last,
                          difference_type_t<ForwardIterator> size,
                          Compare compare, Projection projection)
        -> void
    {
        // Hack to get the stable bidirectional version of vergesort
        // to work correctly without duplicating tons of code
        quick_merge_sort(first.base(), last.base(), size,
                         std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_QUICK_MERGE_SORT_H_
