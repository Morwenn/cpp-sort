/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2015 Orson Peters
    Modified in 2015-2016 by Morwenn for inclusion into cpp-sort

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#ifndef CPPSORT_DETAIL_PDQSORT_H_
#define CPPSORT_DETAIL_PDQSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/bitops.h>
#include "heap_operations.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "iter_sort3.h"

namespace cppsort
{
namespace detail
{
    namespace pdqsort_detail {
        enum {
            // Partitions below this size are sorted using insertion sort.
            insertion_sort_threshold = 24,

            // When we detect an already sorted partition, attempt an insertion sort that allows this
            // amount of element moves before giving up.
            partial_insertion_sort_limit = 8
        };

        // Sorts [begin, end) using insertion sort with the given comparison function. Assumes
        // *(begin - 1) is an element smaller than or equal to any element in [begin, end).
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto unguarded_insertion_sort(RandomAccessIterator begin, RandomAccessIterator end,
                                      Compare comp, Projection projection)
            -> void
        {
            using value_type = value_type_t<RandomAccessIterator>;
            if (begin == end) return;

            auto&& proj = utility::as_function(projection);

            for (RandomAccessIterator cur = begin + 1; cur != end; ++cur) {
                RandomAccessIterator sift = cur;
                RandomAccessIterator sift_1 = cur - 1;

                // Compare first so we can avoid 2 moves for an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    value_type tmp = std::move(*sift);
                    auto&& tmp_proj = proj(tmp);

                    do { *sift-- = std::move(*sift_1); }
                    while (comp(tmp_proj, proj(*--sift_1)));

                    *sift = std::move(tmp);
                }
            }
        }

        // Attempts to use insertion sort on [begin, end). Will return false if more than
        // partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
        // successfully sort and return true.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partial_insertion_sort(RandomAccessIterator begin, RandomAccessIterator end,
                                    Compare comp, Projection projection)
            -> bool
        {
            using value_type = value_type_t<RandomAccessIterator>;
            if (begin == end) return true;

            auto&& proj = utility::as_function(projection);

            int limit = 0;
            for (RandomAccessIterator cur = begin + 1; cur != end; ++cur) {
                if (limit > partial_insertion_sort_limit) return false;

                RandomAccessIterator sift = cur;
                RandomAccessIterator sift_1 = cur - 1;

                // Compare first so we can avoid 2 moves for an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    value_type tmp = std::move(*sift);
                    auto&& tmp_proj = proj(tmp);

                    do { *sift-- = std::move(*sift_1); }
                    while (sift != begin && comp(tmp_proj, proj(*--sift_1)));

                    *sift = std::move(tmp);
                    limit += cur - sift;
                }
            }

            return true;
        }

        // Attempts to use insertion sort on [begin, end). Will return false if more than
        // partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
        // successfully sort and return true. Assumes *(begin - 1) is an element smaller than or
        // equal to any element in [begin, end).
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto unguarded_partial_insertion_sort(RandomAccessIterator begin, RandomAccessIterator end,
                                              Compare comp, Projection projection)
            -> bool
        {
            using value_type = value_type_t<RandomAccessIterator>;
            if (begin == end) return true;

            auto&& proj = utility::as_function(projection);

            int limit = 0;
            for (RandomAccessIterator cur = begin + 1; cur != end; ++cur) {
                if (limit > partial_insertion_sort_limit) return false;

                RandomAccessIterator sift = cur;
                RandomAccessIterator sift_1 = cur - 1;

                // Compare first so we can avoid 2 moves for an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    value_type tmp = std::move(*sift);
                    auto&& tmp_proj = proj(tmp);

                    do { *sift-- = std::move(*sift_1); }
                    while (comp(tmp_proj, proj(*--sift_1)));

                    *sift = std::move(tmp);
                    limit += cur - sift;
                }
            }

            return true;
        }

        // Partitions [begin, end) around pivot *begin using comparison function comp. Elements equal
        // to the pivot are put in the right-hand partition. Returns the position of the pivot after
        // partitioning and whether the passed sequence already was correctly partitioned. Assumes the
        // pivot is a median of at least 3 elements and that [begin, end) is at least
        // insertion_sort_threshold long.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partition_right(RandomAccessIterator begin, RandomAccessIterator end,
                             Compare comp, Projection projection)
            -> std::pair<RandomAccessIterator, bool>
        {
            using value_type = value_type_t<RandomAccessIterator>;
            auto&& proj = utility::as_function(projection);

            // Move pivot into local for speed.
            value_type pivot(std::move(*begin));
            auto&& pivot_proj = proj(pivot);

            RandomAccessIterator first = begin;
            RandomAccessIterator last = end;

            // Find the first element greater than or equal than the pivot (the median of 3 guarantees
            // this exists).
            while (comp(proj(*++first), pivot_proj));

            // Find the first element strictly smaller than the pivot. We have to guard this search if
            // there was no element before *first.
            if (first - 1 == begin) while (first < last && !comp(proj(*--last), pivot_proj));
            else                    while (                !comp(proj(*--last), pivot_proj));

            // If the first pair of elements that should be swapped to partition are the same element,
            // the passed in sequence already was correctly partitioned.
            bool already_partitioned = first >= last;

            // Keep swapping pairs of elements that are on the wrong side of the pivot. Previously
            // swapped pairs guard the searches, which is why the first iteration is special-cased
            // above.
            while (first < last) {
                std::iter_swap(first, last);
                while (comp(proj(*++first), pivot_proj));
                while (!comp(proj(*--last), pivot_proj));
            }

            // Put the pivot in the right place.
            RandomAccessIterator pivot_pos = first - 1;
            *begin = std::move(*pivot_pos);
            *pivot_pos = std::move(pivot);

            return std::make_pair(pivot_pos, already_partitioned);
        }

        // Similar function to the one above, except elements equal to the pivot are put to the left of
        // the pivot and it doesn't check or return if the passed sequence already was partitioned.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partition_left(RandomAccessIterator begin, RandomAccessIterator end,
                            Compare comp, Projection projection)
            -> RandomAccessIterator
        {
            using value_type = value_type_t<RandomAccessIterator>;
            auto&& proj = utility::as_function(projection);

            value_type pivot(std::move(*begin));
            auto&& pivot_proj = proj(pivot);
            RandomAccessIterator first = begin;
            RandomAccessIterator last = end;

            while (comp(pivot_proj, proj(*--last)));

            if (last + 1 == end) while (first < last && !comp(pivot_proj, proj(*++first)));
            else                 while (                !comp(pivot_proj, proj(*++first)));

            while (first < last) {
                std::iter_swap(first, last);
                while (comp(pivot_proj, proj(*--last)));
                while (!comp(pivot_proj, proj(*++first)));
            }

            RandomAccessIterator pivot_pos = last;
            *begin = std::move(*pivot_pos);
            *pivot_pos = std::move(pivot);

            return pivot_pos;
        }


        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto pdqsort_loop(RandomAccessIterator begin, RandomAccessIterator end,
                          Compare comp, Projection projection,
                          int bad_allowed, bool leftmost=true)
            -> void
        {
            using difference_type = difference_type_t<RandomAccessIterator>;
            auto&& proj = utility::as_function(projection);

            // Use a while loop for tail recursion elimination.
            while (true) {
                difference_type size = std::distance(begin, end);

                // Insertion sort is faster for small arrays.
                if (size < insertion_sort_threshold) {
                    if (leftmost) insertion_sort(begin, end, comp, projection);
                    else unguarded_insertion_sort(begin, end, comp, projection);
                    return;
                }

                // Choose pivot as median of 3.
                iter_sort3(begin + size / 2, begin, end - 1, comp, projection);

                // If *(begin - 1) is the end of the right partition of a previous partition operation
                // there is no element in [*begin, end) that is smaller than *(begin - 1). Then if our
                // pivot compares equal to *(begin - 1) we change strategy, putting equal elements in
                // the left partition, greater elements in the right partition. We do not have to
                // recurse on the left partition, since it's sorted (all equal).
                if (!leftmost && !comp(proj(*(begin - 1)), proj(*begin))) {
                    begin = partition_left(begin, end, comp, projection) + 1;
                    continue;
                }

                // Partition and get results.
                std::pair<RandomAccessIterator, bool> part_result
                    = partition_right(begin, end, comp, projection);
                RandomAccessIterator pivot_pos = part_result.first;
                bool already_partitioned = part_result.second;

                // Check for a highly unbalanced partition.
                difference_type pivot_offset = std::distance(begin, pivot_pos);
                bool highly_unbalanced = pivot_offset < size / 8 || pivot_offset > (size - size / 8);

                // If we got a highly unbalanced partition we shuffle elements to break many patterns.
                if (highly_unbalanced) {
                    // If we had too many bad partitions, switch to heapsort to guarantee O(n log n).
                    if (--bad_allowed == 0) {
                        make_heap(begin, end, comp, projection);
                        sort_heap(begin, end, comp, projection);
                        return;
                    }

                    difference_type partition_size = std::distance(begin, pivot_pos);
                    if (partition_size >= insertion_sort_threshold) {
                        std::iter_swap(begin, begin + partition_size / 4);
                        std::iter_swap(pivot_pos - 1, pivot_pos - partition_size / 4);
                    }

                    partition_size = end - pivot_pos;
                    if (partition_size >= insertion_sort_threshold) {
                        std::iter_swap(pivot_pos + 1, pivot_pos + partition_size / 4);
                        std::iter_swap(end - 1, end - partition_size / 4);
                    }
                } else {
                    // If we were decently balanced and we tried to sort an already partitioned
                    // sequence try to use insertion sort.
                    if (already_partitioned &&
                        partial_insertion_sort(begin, pivot_pos, comp, projection) &&
                        unguarded_partial_insertion_sort(pivot_pos + 1, end, comp, projection)) {
                        return;
                    }
                }

                // Sort the left partition first using recursion and do tail recursion elimination for
                // the right-hand partition.
                pdqsort_loop(begin, pivot_pos, comp, projection, bad_allowed, leftmost);
                begin = pivot_pos + 1;
                leftmost = false;
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto pdqsort(RandomAccessIterator begin, RandomAccessIterator end,
                 Compare comp, Projection projection)
        -> void
    {
        if (std::distance(begin, end) < 2) return;
        pdqsort_detail::pdqsort_loop(begin, end,
                                     comp, projection,
                                     utility::log2(end - begin));
    }
}}

#endif // CPPSORT_DETAIL_PDQSORT_H_
