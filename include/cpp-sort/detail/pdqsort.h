/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2021 Orson Peters

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
#include <cstddef>
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include <cpp-sort/utility/iter_move.h>
#include "bitops.h"
#include "heapsort.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "iter_sort3.h"

#ifdef __MINGW32__
#   include <cstdint> // std::uintptr_t
#endif

namespace cppsort
{
namespace detail
{
    namespace pdqsort_detail {
        enum {
            // Partitions below this size are sorted using insertion sort.
            insertion_sort_threshold = 24,

            // Partitions above this size use Tukey's ninther to select the pivot.
            ninther_threshold = 128,

            // When we detect an already sorted partition, attempt an insertion sort that allows this
            // amount of element moves before giving up.
            partial_insertion_sort_limit = 8,

            // Must be multiple of 8 due to loop unrolling, and < 256 to fit in unsigned char.
            block_size = 64,

            // Cacheline size, assumes power of two.
            cacheline_size = 64
        };

        // Sorts [begin, end) using insertion sort with the given comparison function. Assumes
        // *(begin - 1) is an element smaller than or equal to any element in [begin, end).
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto unguarded_insertion_sort(RandomAccessIterator begin, RandomAccessIterator end,
                                      Compare compare, Projection projection)
            -> void
        {
            if (begin == end) return;

            using utility::iter_move;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            for (auto cur = std::next(begin); cur != end; ++cur) {
                auto sift = cur;
                auto sift_1 = std::prev(cur);

                // Compare first so we can avoid 2 moves for an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    auto tmp = iter_move(sift);
                    auto&& tmp_proj = proj(tmp);

                    do {
                        *sift = iter_move(sift_1);
                        --sift;
                    } while (comp(tmp_proj, proj(*--sift_1)));

                    *sift = std::move(tmp);
                }
            }
        }

        // Attempts to use insertion sort on [begin, end). Will return false if more than
        // partial_insertion_sort_limit elements were moved, and abort sorting. Otherwise it will
        // successfully sort and return true.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partial_insertion_sort(RandomAccessIterator begin, RandomAccessIterator end,
                                    Compare compare, Projection projection)
            -> bool
        {
            if (begin == end) return true;

            using utility::iter_move;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            difference_type_t<RandomAccessIterator> limit = 0;
            for (auto cur = std::next(begin); cur != end; ++cur) {
                auto sift = cur;
                auto sift_1 = std::prev(cur);

                // Compare first so we can avoid 2 moves for an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    auto tmp = iter_move(sift);
                    auto&& tmp_proj = proj(tmp);

                    do {
                        *sift = iter_move(sift_1);
                    } while (--sift != begin && comp(tmp_proj, proj(*--sift_1)));

                    *sift = std::move(tmp);
                    limit += cur - sift;
                }

                if (limit > partial_insertion_sort_limit) return false;
            }

            return true;
        }

#ifdef __MINGW32__
        template<typename T>
        auto align_cacheline(T* ptr)
            -> T*
        {
#ifdef UINTPTR_MAX
            std::uintptr_t ip = reinterpret_cast<std::uintptr_t>(ptr);
#else
            std::size_t ip = reinterpret_cast<std::size_t>(ptr);
#endif
            ip = (ip + cacheline_size - 1) & -cacheline_size;
            return reinterpret_cast<T*>(ip);
        }
#endif

        template<typename RandomAccessIterator>
        auto swap_offsets(RandomAccessIterator first, RandomAccessIterator last,
                          unsigned char* offsets_l, unsigned char* offsets_r,
                          std::size_t num, bool use_swaps)
            -> void
        {
            using utility::iter_move;
            using utility::iter_swap;

            if (use_swaps) {
                // This case is needed for the descending distribution, where we need
                // to have proper swapping for pdqsort to remain O(n).
                for (std::size_t i = 0 ; i < num ; ++i) {
                    iter_swap(first + offsets_l[i], last - offsets_r[i]);
                }
            } else if (num > 0) {
                RandomAccessIterator l = first + offsets_l[0];
                RandomAccessIterator r = last - offsets_r[0];
                auto tmp = iter_move(l);
                *l = iter_move(r);
                for (std::size_t i = 1 ; i < num ; ++i) {
                    l = first + offsets_l[i];
                    *r = iter_move(l);
                    r = last - offsets_r[i];
                    *l = iter_move(r);
                }
                *r = std::move(tmp);
            }
        }

        // Partitions [begin, end) around pivot *begin using comparison function compare. Elements equal
        // to the pivot are put in the right-hand partition. Returns the position of the pivot after
        // partitioning and whether the passed sequence already was correctly partitioned. Assumes the
        // pivot is a median of at least 3 elements and that [begin, end) is at least
        // insertion_sort_threshold long. Uses branchless partitioning.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partition_right_branchless(RandomAccessIterator begin, RandomAccessIterator end,
                                        Compare compare, Projection projection)
            -> std::pair<RandomAccessIterator, bool>
        {
            using utility::iter_move;
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Move pivot into local for speed.
            auto pivot = iter_move(begin);
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
            if (!already_partitioned) {
                iter_swap(first, last);
                ++first;

                // The following branchless partitioning is derived from "BlockQuicksort: How Branch
                // Mispredictions don't affect Quicksort" by Stefan Edelkamp and Armin Weiss, but
                // heavily micro-optimized.
#ifdef __MINGW32__
                unsigned char offsets_l_storage[block_size + cacheline_size];
                unsigned char offsets_r_storage[block_size + cacheline_size];
                unsigned char* offsets_l = align_cacheline(offsets_l_storage);
                unsigned char* offsets_r = align_cacheline(offsets_r_storage);
#else
                alignas(cacheline_size) unsigned char offsets_l_storage[block_size];
                alignas(cacheline_size) unsigned char offsets_r_storage[block_size];
                unsigned char* offsets_l = offsets_l_storage;
                unsigned char* offsets_r = offsets_r_storage;
#endif

                auto offsets_l_base = first;
                auto offsets_r_base = last;
                std::size_t num_l, num_r, start_l, start_r;
                num_l = num_r = start_l = start_r = 0;

                while (first < last) {
                    // Fill up offset blocks with elements that are on the wrong side.
                    // First we determine how much elements are considered for each offset block.
                    std::size_t num_unknown = last - first;
                    std::size_t left_split = num_l == 0 ? (num_r == 0 ? num_unknown / 2 : num_unknown) : 0;
                    std::size_t right_split = num_r == 0 ? (num_unknown - left_split) : 0;

                    // Fill the offset blocks.
                    if (left_split >= block_size) {
                        for (std::size_t i = 0 ; i < block_size ;) {
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                            offsets_l[num_l] = i++; num_l += not comp(proj(*first), pivot_proj); ++first;
                        }
                    } else {
                        for (std::size_t i = 0 ; i < left_split ;) {
                            offsets_l[num_l] = i++;
                            num_l += not comp(proj(*first), pivot_proj);
                            ++first;
                        }
                    }

                    if (right_split >= block_size) {
                        for (std::size_t i = 0 ; i < block_size ;) {
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                            offsets_r[num_r] = ++i; num_r += comp(proj(*--last), pivot_proj);
                        }
                    } else {
                        for (std::size_t i = 0 ; i < right_split ;) {
                            offsets_r[num_r] = ++i;
                            num_r += comp(proj(*--last), pivot_proj);
                        }
                    }

                    // Swap elements and update block sizes and first/last boundaries.
                    std::size_t num = (std::min)(num_l, num_r);
                    swap_offsets(offsets_l_base, offsets_r_base,
                                 offsets_l + start_l, offsets_r + start_r,
                                 num, num_l == num_r);
                    num_l -= num;
                    num_r -= num;
                    start_l += num;
                    start_r += num;

                    if (num_l == 0) {
                        start_l = 0;
                        offsets_l_base = first;
                    }

                    if (num_r == 0) {
                        start_r = 0;
                        offsets_r_base = last;
                    }
                }

                // We have now fully identified [first, last)'s proper position. Swap the last elements.
                if (num_l) {
                    offsets_l += start_l;
                    while (num_l--) {
                        iter_swap(offsets_l_base + offsets_l[num_l], --last);
                    }
                    first = last;
                }
                if (num_r) {
                    offsets_r += start_r;
                    while (num_r--) {
                        iter_swap(offsets_r_base - offsets_r[num_r], first);
                        ++first;
                    }
                    last = first;
                }
            }

            // Put the pivot in the right place.
            auto pivot_pos = first - 1;
            *begin = iter_move(pivot_pos);
            *pivot_pos = std::move(pivot);

            return std::make_pair(pivot_pos, already_partitioned);
        }

        // Partitions [begin, end) around pivot *begin using comparison function compare. Elements equal
        // to the pivot are put in the right-hand partition. Returns the position of the pivot after
        // partitioning and whether the passed sequence already was correctly partitioned. Assumes the
        // pivot is a median of at least 3 elements and that [begin, end) is at least
        // insertion_sort_threshold long.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partition_right(RandomAccessIterator begin, RandomAccessIterator end,
                             Compare compare, Projection projection)
            -> std::pair<RandomAccessIterator, bool>
        {
            using utility::iter_move;
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Move pivot into local for speed.
            auto pivot = iter_move(begin);
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
                iter_swap(first, last);
                while (comp(proj(*++first), pivot_proj));
                while (!comp(proj(*--last), pivot_proj));
            }

            // Put the pivot in the right place.
            RandomAccessIterator pivot_pos = first - 1;
            *begin = iter_move(pivot_pos);
            *pivot_pos = std::move(pivot);

            return std::make_pair(pivot_pos, already_partitioned);
        }

        // Similar function to the one above, except elements equal to the pivot are put to the left of
        // the pivot and it doesn't check or return if the passed sequence already was partitioned.
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto partition_left(RandomAccessIterator begin, RandomAccessIterator end,
                            Compare compare, Projection projection)
            -> RandomAccessIterator
        {
            using utility::iter_move;
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            auto pivot = iter_move(begin);
            auto&& pivot_proj = proj(pivot);
            RandomAccessIterator first = begin;
            RandomAccessIterator last = end;

            while (comp(pivot_proj, proj(*--last)));

            if (last + 1 == end) while (first < last && !comp(pivot_proj, proj(*++first)));
            else                 while (                !comp(pivot_proj, proj(*++first)));

            while (first < last) {
                iter_swap(first, last);
                while (comp(pivot_proj, proj(*--last)));
                while (!comp(pivot_proj, proj(*++first)));
            }

            RandomAccessIterator pivot_pos = last;
            *begin = iter_move(pivot_pos);
            *pivot_pos = std::move(pivot);

            return pivot_pos;
        }


        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto pdqsort_loop(RandomAccessIterator begin, RandomAccessIterator end,
                          Compare compare, Projection projection,
                          int bad_allowed, bool leftmost=true)
            -> void
        {
            using utility::iter_swap;
            using difference_type = difference_type_t<RandomAccessIterator>;
            using value_type = value_type_t<RandomAccessIterator>;
            using projected_type = projected_t<RandomAccessIterator, Projection>;

            constexpr bool is_branchless =
                utility::is_probably_branchless_comparison_v<Compare, projected_type> &&
                utility::is_probably_branchless_projection_v<Projection, value_type>;
            (void)is_branchless; // Silence a -Wunused-but-set-variable false positive

            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // Use a while loop for tail recursion elimination.
            while (true) {
                difference_type size = end - begin;

                // Insertion sort is faster for small arrays.
                if (size < insertion_sort_threshold) {
                    if (leftmost) {
                        insertion_sort(begin, end, std::move(compare), std::move(projection));
                    } else {
                        unguarded_insertion_sort(begin, end, std::move(compare), std::move(projection));
                    }
                    return;
                }

                // Choose pivot as median of 3 or pseudomedian of 9.
                difference_type s2 = size / 2;
                if (size > ninther_threshold) {
                    iter_sort3(begin, begin + s2, end - 1, compare, projection);
                    iter_sort3(begin + 1, begin + (s2 - 1), end - 2, compare, projection);
                    iter_sort3(begin + 2, begin + (s2 + 1), end - 3, compare, projection);
                    iter_sort3(begin + (s2 - 1), begin + s2, begin + (s2 + 1), compare, projection);
                    iter_swap(begin, begin + s2);
                } else {
                    iter_sort3(begin + s2, begin, end - 1, compare, projection);
                }

                // If *(begin - 1) is the end of the right partition of a previous partition operation
                // there is no element in [begin, end) that is smaller than *(begin - 1). Then if our
                // pivot compares equal to *(begin - 1) we change strategy, putting equal elements in
                // the left partition, greater elements in the right partition. We do not have to
                // recurse on the left partition, since it's sorted (all equal).
                if (!leftmost && !comp(proj(*(begin - 1)), proj(*begin))) {
                    begin = partition_left(begin, end, compare, projection) + 1;
                    continue;
                }

                // Partition and get results.
                std::pair<RandomAccessIterator, bool> part_result = is_branchless ?
                    partition_right_branchless(begin, end, compare, projection) :
                    partition_right(begin, end, compare, projection);
                RandomAccessIterator pivot_pos = part_result.first;
                bool already_partitioned = part_result.second;

                // Check for a highly unbalanced partition.
                difference_type l_size = pivot_pos - begin;
                difference_type r_size = end - (pivot_pos + 1);
                bool highly_unbalanced = l_size < size / 8 || r_size < size / 8;

                // If we got a highly unbalanced partition we shuffle elements to break many patterns.
                if (highly_unbalanced) {
                    // If we had too many bad partitions, switch to heapsort to guarantee O(n log n).
                    if (--bad_allowed == 0) {
                        heapsort(std::move(begin), std::move(end),
                                 std::move(compare), std::move(projection));
                        return;
                    }

                    if (l_size >= insertion_sort_threshold) {
                        iter_swap(begin,             begin + l_size / 4);
                        iter_swap(pivot_pos - 1, pivot_pos - l_size / 4);

                        if (l_size > ninther_threshold) {
                            iter_swap(begin + 1,         begin + (l_size / 4 + 1));
                            iter_swap(begin + 2,         begin + (l_size / 4 + 2));
                            iter_swap(pivot_pos - 2, pivot_pos - (l_size / 4 + 1));
                            iter_swap(pivot_pos - 3, pivot_pos - (l_size / 4 + 2));
                        }
                    }

                    if (r_size >= insertion_sort_threshold) {
                        iter_swap(pivot_pos + 1, pivot_pos + (1 + r_size / 4));
                        iter_swap(end - 1,                   end - r_size / 4);

                        if (r_size > ninther_threshold) {
                            iter_swap(pivot_pos + 2, pivot_pos + (2 + r_size / 4));
                            iter_swap(pivot_pos + 3, pivot_pos + (3 + r_size / 4));
                            iter_swap(end - 2,             end - (1 + r_size / 4));
                            iter_swap(end - 3,             end - (2 + r_size / 4));
                        }
                    }
                } else {
                    // If we were decently balanced and we tried to sort an already partitioned
                    // sequence try to use insertion sort.
                    if (already_partitioned &&
                        partial_insertion_sort(begin, pivot_pos, compare, projection) &&
                        partial_insertion_sort(pivot_pos + 1, end, compare, projection)) {
                        return;
                    }
                }

                // Sort the left partition first using recursion and do tail recursion elimination for
                // the right-hand partition.
                pdqsort_loop(begin, pivot_pos, compare, projection, bad_allowed, leftmost);
                begin = pivot_pos + 1;
                leftmost = false;
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto pdqsort(RandomAccessIterator begin, RandomAccessIterator end,
                 Compare compare, Projection projection)
        -> void
    {
        auto size = end - begin;
        if (size < 2) return;

        pdqsort_detail::pdqsort_loop(std::move(begin), std::move(end),
                                     std::move(compare), std::move(projection),
                                     detail::log2(size));
    }
}}

#endif // CPPSORT_DETAIL_PDQSORT_H_
