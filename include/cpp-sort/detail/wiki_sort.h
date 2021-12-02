/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
 * WikiSort: a public domain implementation of "Block Sort"
 * https://github.com/BonzaiThePenguin/WikiSort
 */
#ifndef CPPSORT_DETAIL_WIKI_SORT_H_
#define CPPSORT_DETAIL_WIKI_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "bitops.h"
#include "buffered_inplace_merge.h"
#include "config.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "merge_move.h"
#include "move.h"
#include "rotate.h"
#include "swap_ranges.h"
#include "type_traits.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator>
    struct Range
    {
        Iterator start;
        Iterator end;

        Range() {}

        Range(Iterator start, Iterator end):
            start(std::move(start)),
            end(std::move(end))
        {}

        auto length() const
            -> difference_type_t<Iterator>
        {
            return end - start;
        }
    };

    // combine a linear search with a binary search to reduce the number of comparisons in situations
    // where have some idea as to how many unique values there are and where the next value might be
    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindFirstForward(RandomAccessIterator first, RandomAccessIterator last, T&& value,
                          Compare compare, Projection projection,
                          difference_type_t<RandomAccessIterator> unique)
        -> RandomAccessIterator
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        difference_type size = last - first;
        if (size == 0) return first;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(std::forward<T>(value));
        difference_type skip = std::max<difference_type>(size / unique, 1);

        RandomAccessIterator index;
        for (index = first + skip ; comp(proj(*std::prev(index)), value_proj) ; index += skip) {
            if (index >= last - skip) {
                return lower_bound(index, last, value_proj,
                                   std::move(compare), std::move(projection));
            }
        }
        return lower_bound(index - skip, index, value_proj,
                           std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindLastForward(RandomAccessIterator first, RandomAccessIterator last, T&& value,
                         Compare compare, Projection projection,
                         difference_type_t<RandomAccessIterator> unique)
        -> RandomAccessIterator
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        difference_type size = last - first;
        if (size == 0) return first;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(std::forward<T>(value));
        difference_type skip = std::max<difference_type>(size / unique, 1);

        RandomAccessIterator index;
        for (index = first + skip ; not comp(value_proj, proj(*std::prev(index))) ; index += skip) {
            if (index >= last - skip) {
                return upper_bound(index, last, value_proj,
                                   std::move(compare), std::move(projection));
            }
        }
        return upper_bound(index - skip, index, value_proj,
                           std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindFirstBackward(RandomAccessIterator first, RandomAccessIterator last, T&& value,
                           Compare compare, Projection projection,
                           difference_type_t<RandomAccessIterator> unique)
        -> RandomAccessIterator
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        difference_type size = last - first;
        if (size == 0) return first;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(std::forward<T>(value));
        difference_type skip = std::max<difference_type>(size / unique, 1);

        RandomAccessIterator index;
        for (index = last - skip ; index > first && not comp(proj(*std::prev(index)), value_proj) ; index -= skip) {
            if (index < first + skip) {
                return lower_bound(first, index, value_proj,
                                   std::move(compare), std::move(projection));
            }
        }
        return lower_bound_n(index, skip, value_proj, std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindLastBackward(RandomAccessIterator first, RandomAccessIterator last, T&& value,
                          Compare compare, Projection projection,
                          difference_type_t<RandomAccessIterator> unique)
        -> RandomAccessIterator
    {
        using difference_type = difference_type_t<RandomAccessIterator>;

        difference_type size = last - first;
        if (size == 0) return first;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(std::forward<T>(value));
        difference_type skip = std::max<difference_type>(size / unique, 1);

        RandomAccessIterator index;
        for (index = last - skip ; index > first && comp(value_proj, proj(*std::prev(index))) ; index -= skip) {
            if (index < first + skip) {
                return upper_bound(first, index, value_proj,
                                   std::move(compare), std::move(projection));
            }
        }
        return upper_bound_n(index, skip, value_proj, std::move(compare), std::move(projection));
    }

    namespace Wiki
    {
        // merge operation using an internal buffer
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto MergeInternal(RandomAccessIterator first1, RandomAccessIterator last1,
                           RandomAccessIterator first2, RandomAccessIterator last2,
                           RandomAccessIterator buffer,
                           Compare compare, Projection projection)
            -> void
        {
            using utility::iter_swap;

            // whenever we find a value to add to the final array, swap it with the value that's already in that spot
            // when this algorithm is finished, 'buffer' will contain its original contents, but in a different order
            RandomAccessIterator A_index = buffer;
            RandomAccessIterator B_index = first2;
            RandomAccessIterator A_last = buffer + (last1 - first1);
            RandomAccessIterator B_last = last2;
            RandomAccessIterator insert_index = first1;

            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (first1 != last1 && first2 != last2) {
                while (true) {
                    if (not comp(proj(*B_index), proj(*A_index))) {
                        iter_swap(insert_index, A_index);
                        ++A_index;
                        ++insert_index;
                        if (A_index == A_last) break;
                    } else {
                        iter_swap(insert_index, B_index);
                        ++B_index;
                        ++insert_index;
                        if (B_index == B_last) break;
                    }
                }
            }

            // BlockSwap
            detail::swap_ranges_overlap(std::move(A_index), std::move(A_last), std::move(insert_index));
        }

        // merge operation without a buffer
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto MergeInPlace(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
                          Compare compare, Projection projection)
            -> void
        {
            if (first == middle || middle == last) return;

            /*
             this just repeatedly binary searches into B and rotates A into position.
             the paper suggests using the 'rotation-based Hwang and Lin algorithm' here,
             but I decided to stick with this because it had better situational performance

             (Hwang and Lin is designed for merging subarrays of very different sizes,
             but WikiSort almost always uses subarrays that are roughly the same size)

             normally this is incredibly suboptimal, but this function is only called
             when none of the A or B blocks in any subarray contained 2√A unique values,
             which places a hard limit on the number of times this will ACTUALLY need
             to binary search and rotate.

             according to my analysis the worst case is √A rotations performed on √A items
             once the constant factors are removed, which ends up being O(n)

             again, this is NOT a general-purpose solution – it only works well in this case!
             kind of like how the O(n^2) insertion sort is used in some places
             */
            auto&& proj = utility::as_function(projection);

            while (true) {
                // find the first place in B where the first item in A needs to be inserted
                RandomAccessIterator mid2 = lower_bound(middle, last, proj(*first), compare, projection);

                // rotate A into place
                auto amount = mid2 - middle;
                detail::rotate(first, middle, mid2);
                if (last == mid2) break;

                // calculate the new A and B ranges
                middle = mid2;
                first += amount;
                first = upper_bound(first, middle, proj(*first), compare, projection);
                if (first == middle) break;
            }
        }

        // calculate how to scale the index value to the range within the array
        // the bottom-up merge sort only operates on values that are powers of two,
        // so scale down to that power of two, then use a fraction to scale back again
        template<typename Iter>
        class Iterator
        {
            using iterator = Iter;
            using difference_type = difference_type_t<iterator>;

            difference_type size, power_of_two;
            difference_type decimal, numerator, denominator;
            difference_type decimal_step, numerator_step;

        public:

            Iterator(difference_type size, difference_type min_level):
                size(size),
                power_of_two(hyperfloor(size)),
                decimal(0),
                numerator(0),
                denominator(power_of_two / min_level),
                decimal_step(size / denominator),
                numerator_step(size % denominator)
            {}

            auto begin()
                -> void
            {
                numerator = decimal = 0;
            }

            auto nextRange(iterator it)
                -> Range<iterator>
            {
                difference_type start = decimal;

                decimal += decimal_step;
                numerator += numerator_step;
                if (numerator >= denominator) {
                    numerator -= denominator;
                    ++decimal;
                }

                return { it + start, it + decimal };
            }

            auto finished() const
                -> bool
            {
                return decimal >= size;
            }

            auto nextLevel()
                -> bool
            {
                decimal_step += decimal_step;
                numerator_step += numerator_step;
                if (numerator_step >= denominator) {
                    numerator_step -= denominator;
                    ++decimal_step;
                }

                return decimal_step < size;
            }

            auto length() const
                -> difference_type
            {
                return decimal_step;
            }
        };

        // bottom-up merge sort combined with an in-place merge algorithm for O(1) memory use,
        template<typename RandomAccessIterator, typename CacheIterator,
                 typename Compare, typename Projection>
        auto sort(RandomAccessIterator first, RandomAccessIterator last,
                  CacheIterator cache_begin, difference_type_t<CacheIterator> cache_size,
                  Compare compare, Projection projection)
            -> void
        {
            using utility::iter_swap;
            using difference_type = difference_type_t<RandomAccessIterator>;

            difference_type size = last - first;
            if (size < 15) {
                insertion_sort(std::move(first), std::move(last),
                               std::move(compare), std::move(projection));
                return;
            }

            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            // sort groups of 4-8 items at a time using an unstable sorting network,
            // but keep track of the original item orders to force it to be stable
            // http://pages.ripco.net/~jgamble/nw.html
            Wiki::Iterator<RandomAccessIterator> iterator(size, 4);
            while (not iterator.finished()) {
                std::uint8_t order[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
                auto range = iterator.nextRange(first);

                auto do_swap = [&](difference_type x, difference_type y) {
                    if (comp(proj(range.start[y]), proj(range.start[x])) ||
                        (order[x] > order[y] && not comp(proj(range.start[x]), proj(range.start[y])))
                    ) {
                        iter_swap(range.start + x, range.start + y);
                        iter_swap(order + x, order + y);
                    }
                };

                if (range.length() == 8) {
                    do_swap(0, 1); do_swap(2, 3); do_swap(4, 5); do_swap(6, 7);
                    do_swap(0, 2); do_swap(1, 3); do_swap(4, 6); do_swap(5, 7);
                    do_swap(1, 2); do_swap(5, 6); do_swap(0, 4); do_swap(3, 7);
                    do_swap(1, 5); do_swap(2, 6);
                    do_swap(1, 4); do_swap(3, 6);
                    do_swap(2, 4); do_swap(3, 5);
                    do_swap(3, 4);

                } else if (range.length() == 7) {
                    do_swap(1, 2); do_swap(3, 4); do_swap(5, 6);
                    do_swap(0, 2); do_swap(3, 5); do_swap(4, 6);
                    do_swap(0, 1); do_swap(4, 5); do_swap(2, 6);
                    do_swap(0, 4); do_swap(1, 5);
                    do_swap(0, 3); do_swap(2, 5);
                    do_swap(1, 3); do_swap(2, 4);
                    do_swap(2, 3);

                } else if (range.length() == 6) {
                    do_swap(1, 2); do_swap(4, 5);
                    do_swap(0, 2); do_swap(3, 5);
                    do_swap(0, 1); do_swap(3, 4); do_swap(2, 5);
                    do_swap(0, 3); do_swap(1, 4);
                    do_swap(2, 4); do_swap(1, 3);
                    do_swap(2, 3);

                } else if (range.length() == 5) {
                    do_swap(0, 1); do_swap(3, 4);
                    do_swap(2, 4);
                    do_swap(2, 3); do_swap(1, 4);
                    do_swap(0, 3);
                    do_swap(0, 2); do_swap(1, 3);
                    do_swap(1, 2);

                } else if (range.length() == 4) {
                    do_swap(0, 1); do_swap(2, 3);
                    do_swap(0, 2); do_swap(1, 3);
                    do_swap(1, 2);
                }
            }

            // merge sort the higher levels, which can be 8-15, 16-31, 32-63, 64-127, etc.
            while (true) {
                // if every A and B block will fit into the cache, use a special branch specifically for merging with the cache
                // (we use < rather than <= since the block size might be one more than iterator.length())
                if (iterator.length() < cache_size) {

                    // if four subarrays fit into the cache, it's faster to merge both pairs of subarrays into the cache,
                    // then merge the two merged subarrays from the cache back into the original array
                    if ((iterator.length() + 1) * 4 <= cache_size && iterator.length() * 4 <= size) {
                        iterator.begin();
                        while (not iterator.finished()) {
                            // merge A1 and B1 into the cache
                            auto A1 = iterator.nextRange(first);
                            auto B1 = iterator.nextRange(first);
                            auto A2 = iterator.nextRange(first);
                            auto B2 = iterator.nextRange(first);

                            if (comp(proj(*std::prev(B1.end)), proj(*A1.start))) {
                                // the two ranges are in reverse order, so move them in reverse order into the cache
                                detail::move(A1.start, A1.end, cache_begin + B1.length());
                                detail::move(B1.start, B1.end, cache_begin);
                            } else if (comp(proj(*B1.start), proj(*std::prev(A1.end)))) {
                                // these two ranges weren't already in order, so merge them into the cache
                                merge_move(A1.start, A1.end, B1.start, B1.end, cache_begin,
                                           compare, projection, projection);
                            } else {
                                // if A1, B1, A2, and B2 are all in order, skip doing anything else
                                if (not comp(proj(*B2.start), proj(*std::prev(A2.end))) &&
                                    not comp(proj(*A2.start), proj(*std::prev(B1.end)))) continue;

                                // move A1 and B1 into the cache in the same order
                                detail::move(A1.start, B1.end, cache_begin);
                            }
                            A1 = { A1.start, B1.end };

                            // merge A2 and B2 into the cache
                            if (comp(proj(*std::prev(B2.end)), proj(*A2.start))) {
                                // the two ranges are in reverse order, so move them in reverse order into the cache
                                detail::move(A2.start, A2.end, cache_begin + (A1.length() + B2.length()));
                                detail::move(B2.start, B2.end, cache_begin + A1.length());
                            } else if (comp(proj(*B2.start), proj(*std::prev(A2.end)))) {
                                // these two ranges weren't already in order, so merge them into the cache
                                merge_move(A2.start, A2.end, B2.start, B2.end, cache_begin + A1.length(),
                                           compare, projection, projection);
                            } else {
                                // move A2 and B2 into the cache in the same order
                                detail::move(A2.start, B2.end, cache_begin + A1.length());
                            }
                            A2 = { A2.start, B2.end };

                            // merge A1 and A2 from the cache into the array
                            Range<CacheIterator> A3 = { cache_begin, cache_begin + A1.length() };
                            Range<CacheIterator> B3 = {
                                cache_begin + A1.length(),
                                cache_begin + (A1.length() + A2.length())
                            };

                            if (comp(proj(*std::prev(B3.end)), proj(*A3.start))) {
                                // the two ranges are in reverse order, so move them in reverse order into the array
                                detail::move(A3.start, A3.end, A1.start + A2.length());
                                detail::move(B3.start, B3.end, A1.start);
                            } else if (comp(proj(*B3.start), proj(*std::prev(A3.end)))) {
                                // these two ranges weren't already in order, so merge them back into the array
                                merge_move(A3.start, A3.end, B3.start, B3.end, A1.start,
                                           compare, projection, projection);
                            } else {
                                // move A3 and B3 into the array in the same order
                                detail::move(A3.start, B3.end, A1.start);
                            }
                        }

                        // we merged two levels at the same time, so we're done with this level already
                        // (iterator.nextLevel() is called again at the bottom of this outer merge loop)
                        iterator.nextLevel();

                    } else {
                        iterator.begin();
                        while (not iterator.finished()) {
                            auto A = iterator.nextRange(first);
                            auto B = iterator.nextRange(first);

                            if (comp(proj(*std::prev(B.end)), proj(*A.start))) {
                                // the two ranges are in reverse order, so a simple rotation should fix it
                                detail::rotate(A.start, A.end, B.end);
                            } else if (comp(proj(*B.start), proj(*std::prev(A.end)))) {
                                // these two ranges weren't already in order, so we need to merge them
                                buffered_inplace_merge(A.start, A.end, B.end, compare, projection,
                                                       A.length(), B.length(), cache_begin);
                            }
                        }
                    }
                } else {
                    // this is where the in-place merge logic starts!
                    // 1. pull out two internal buffers each containing √A unique values
                    //     1a. adjust block_size and buffer_size if we couldn't find enough unique values
                    // 2. loop over the A and B subarrays within this level of the merge sort
                    //     3. break A and B into blocks of size 'block_size'
                    //     4. "tag" each of the A blocks with values from the first internal buffer
                    //     5. roll the A blocks through the B blocks and drop/rotate them where they belong
                    //     6. merge each A block with any B values that follow, using the cache or the second internal buffer
                    // 7. sort the second internal buffer if it exists
                    // 8. redistribute the two internal buffers back into the array

                    difference_type block_size = static_cast<difference_type>(std::sqrt(iterator.length()));
                    difference_type buffer_size = iterator.length() / block_size + 1;

                    // as an optimization, we really only need to pull out the internal buffers once for each level of merges
                    // after that we can reuse the same buffers over and over, then redistribute it when we're finished with this level
                    Range<RandomAccessIterator> buffer1 = { first, first };
                    Range<RandomAccessIterator> buffer2 = { first, first };
                    RandomAccessIterator index, last;
                    difference_type count, pull_index = 0;
                    struct
                    {
                        RandomAccessIterator from, to;
                        difference_type count;
                        Range<RandomAccessIterator> range;
                    } pull[2];
                    pull[0].count = 0; pull[0].range = { first, first };
                    pull[1].count = 0; pull[1].range = { first, first };

                    // find two internal buffers of size 'buffer_size' each
                    // let's try finding both buffers at the same time from a single A or B subarray
                    difference_type find = buffer_size + buffer_size;
                    bool find_separately = false;

                    if (cache_size > 0 && block_size <= cache_size) {
                        // if every A block fits into the cache then we won't need the second internal buffer,
                        // so we really only need to find 'buffer_size' unique values
                        find = buffer_size;
                    } else if (find > iterator.length()) {
                        // we can't fit both buffers into the same A or B subarray, so find two buffers separately
                        find = buffer_size;
                        find_separately = true;
                    }

                    // we need to find either a single contiguous space containing 2√A unique values (which will be split up into two buffers of size √A each),
                    // or we need to find one buffer of < 2√A unique values, and a second buffer of √A unique values,
                    // OR if we couldn't find that many unique values, we need the largest possible buffer we can get

                    // in the case where it couldn't find a single buffer of at least √A unique values,
                    // all of the Merge steps must be replaced by a different merge algorithm (MergeInPlace)

                    iterator.begin();
                    while (not iterator.finished()) {
                        auto A = iterator.nextRange(first);
                        auto B = iterator.nextRange(first);

                        // just store information about where the values will be pulled from and to,
                        // as well as how many values there are, to create the two internal buffers
                        auto do_pull = [&](RandomAccessIterator to) {
                            pull[pull_index].range = { A.start, B.end };
                            pull[pull_index].count = count;
                            pull[pull_index].from = index;
                            pull[pull_index].to = to;
                        };

                        // check A for the number of unique values we need to fill an internal buffer
                        // these values will be pulled out to the start of A
                        for (last = A.start, count = 1; count < find ; last = index, (void) ++count) {
                            index = FindLastForward(std::next(last), A.end, *last, compare, projection, find - count);
                            if (index == A.end) break;
                            CPPSORT_ASSERT(index < A.end);
                        }
                        index = last;

                        if (count >= buffer_size) {
                            // keep track of the range within the array where we'll need to "pull out" these values to create the internal buffer
                            do_pull(A.start);
                            pull_index = 1;

                            if (count == buffer_size + buffer_size) {
                                // we were able to find a single contiguous section containing 2√A unique values,
                                // so this section can be used to contain both of the internal buffers we'll need
                                buffer1 = { A.start, A.start + buffer_size };
                                buffer2 = { A.start + buffer_size, A.start + count };
                                break;
                            } else if (find == buffer_size + buffer_size) {
                                // we found a buffer that contains at least √A unique values, but did not contain the full 2√A unique values,
                                // so we still need to find a second separate buffer of at least √A unique values
                                buffer1 = { A.start, A.start + count };
                                find = buffer_size;
                            } else if (cache_size > 0 && block_size <= cache_size) {
                                // we found the first and only internal buffer that we need, so we're done!
                                buffer1 = { A.start, A.start + count };
                                break;
                            } else if (find_separately) {
                                // found one buffer, but now find the other one
                                buffer1 = { A.start, A.start + count };
                                find_separately = false;
                            } else {
                                // we found a second buffer in an 'A' subarray containing √A unique values, so we're done!
                                buffer2 = { A.start, A.start + count };
                                break;
                            }
                        } else if (pull_index == 0 && count > buffer1.length()) {
                            // keep track of the largest buffer we were able to find
                            buffer1 = { A.start, A.start + count };
                            do_pull(A.start);
                        }

                        // check B for the number of unique values we need to fill an internal buffer
                        // these values will be pulled out to the end of B
                        for (last = std::prev(B.end), count = 1; count < find; last = std::prev(index), (void) ++count) {
                            index = FindFirstBackward(B.start, last, *last, compare, projection, find - count);
                            if (index == B.start) break;
                            CPPSORT_ASSERT(index > B.start);
                        }
                        index = last;

                        if (count >= buffer_size) {
                            // keep track of the range within the array where we'll need to "pull out" these values to create the internal buffer
                            do_pull(B.end);
                            pull_index = 1;

                            if (count == buffer_size + buffer_size) {
                                // we were able to find a single contiguous section containing 2√A unique values,
                                // so this section can be used to contain both of the internal buffers we'll need
                                buffer1 = { B.end - count, B.end - buffer_size };
                                buffer2 = { B.end - buffer_size, B.end };
                                break;
                            } else if (find == buffer_size + buffer_size) {
                                // we found a buffer that contains at least √A unique values, but did not contain the full 2√A unique values,
                                // so we still need to find a second separate buffer of at least √A unique values
                                buffer1 = { B.end - count, B.end };
                                find = buffer_size;
                            } else if (cache_size > 0 && block_size <= cache_size) {
                                // we found the first and only internal buffer that we need, so we're done!
                                buffer1 = { B.end - count, B.end };
                                break;
                            } else if (find_separately) {
                                // found one buffer, but now find the other one
                                buffer1 = { B.end - count, B.end };
                                find_separately = false;
                            } else {
                                // buffer2 will be pulled out from a 'B' subarray, so if the first buffer was pulled out from the corresponding 'A' subarray,
                                // we need to adjust the end point for that A subarray so it knows to stop redistributing its values before reaching buffer2
                                if (pull[0].range.start == A.start) {
                                    pull[0].range.end -= pull[1].count;
                                }

                                // we found a second buffer in a 'B' subarray containing √A unique values, so we're done!
                                buffer2 = { B.end - count, B.end };
                                break;
                            }
                        } else if (pull_index == 0 && count > buffer1.length()) {
                            // keep track of the largest buffer we were able to find
                            buffer1 = { B.end - count, B.end };
                            do_pull(B.end);
                        }
                    }

                    // pull out the two ranges so we can use them as internal buffers
                    for (pull_index = 0; pull_index < 2; ++pull_index) {
                        difference_type length = pull[pull_index].count;

                        if (pull[pull_index].to < pull[pull_index].from) {
                            // we're pulling the values out to the left, which means the start of an A subarray
                            index = pull[pull_index].from;
                            for (count = 1; count < length; ++count) {
                                index = FindFirstBackward(pull[pull_index].to, pull[pull_index].from - (count - 1),
                                                          *std::prev(index), compare, projection, length - count);
                                Range<RandomAccessIterator> range = {
                                    std::next(index),
                                    std::next(pull[pull_index].from)
                                };
                                detail::rotate(range.start, range.end - count, range.end);
                                pull[pull_index].from = index + count;
                            }
                        } else if (pull[pull_index].to > pull[pull_index].from) {
                            // we're pulling values out to the right, which means the end of a B subarray
                            index = std::next(pull[pull_index].from);
                            for (count = 1; count < length; ++count) {
                                index = FindLastForward(index, pull[pull_index].to, *index,
                                                        compare, projection, length - count);
                                Range<RandomAccessIterator> range = { pull[pull_index].from, std::prev(index) };
                                detail::rotate(range.start, range.start + count, range.end);
                                pull[pull_index].from = std::prev(index - count);
                            }
                        }
                    }

                    // adjust block_size and buffer_size based on the values we were able to pull out
                    buffer_size = buffer1.length();
                    block_size = iterator.length() / buffer_size + 1;

                    // the first buffer NEEDS to be large enough to tag each of the evenly sized A blocks,
                    // so this was originally here to test the math for adjusting block_size above
                    //CPPSORT_ASSERT((iterator.length() + 1)/block_size <= buffer_size);

                    // now that the two internal buffers have been created, it's time to merge each A+B combination at this level of the merge sort!
                    iterator.begin();
                    while (not iterator.finished()) {
                        auto A = iterator.nextRange(first);
                        auto B = iterator.nextRange(first);

                        // remove any parts of A or B that are being used by the internal buffers
                        RandomAccessIterator start = A.start;
                        if (start == pull[0].range.start) {
                            if (pull[0].from > pull[0].to) {
                                A.start += pull[0].count;

                                // if the internal buffer takes up the entire A or B subarray, then there's nothing to merge
                                // this only happens for very small subarrays, like √4 = 2, 2 * (2 internal buffers) = 4,
                                // which also only happens when cache size is small or 0 since it'd otherwise use merge_move
                                if (A.length() == 0) continue;
                            } else if (pull[0].from < pull[0].to) {
                                B.end -= pull[0].count;
                                if (B.length() == 0) continue;
                            }
                        }
                        if (start == pull[1].range.start) {
                            if (pull[1].from > pull[1].to) {
                                A.start += pull[1].count;
                                if (A.length() == 0) continue;
                            } else if (pull[1].from < pull[1].to) {
                                B.end -= pull[1].count;
                                if (B.length() == 0) continue;
                            }
                        }

                        if (comp(proj(*std::prev(B.end)), proj(*A.start))) {
                            // the two ranges are in reverse order, so a simple rotation should fix it
                            detail::rotate(A.start, A.end, B.end);
                        } else if (comp(proj(*B.start), proj(*std::prev(A.end)))) {
                            // these two ranges weren't already in order, so we'll need to merge them!

                            // break the remainder of A into blocks. firstA is the uneven-sized first A block
                            Range<RandomAccessIterator> blockA = A;
                            Range<RandomAccessIterator> firstA = { A.start, A.start + blockA.length() % block_size };

                            // swap the first value of each A block with the values in buffer1
                            for (auto indexA = buffer1.start, index = firstA.end ; index < blockA.end ; ++indexA) {
                                iter_swap(indexA, index);
                                index += block_size;
                            }

                            // start rolling the A blocks through the B blocks!
                            // when we leave an A block behind we'll need to merge the previous A block with any B blocks that follow it, so track that information as well
                            Range<RandomAccessIterator> lastA = firstA;
                            Range<RandomAccessIterator> lastB = { first, first };
                            Range<RandomAccessIterator> blockB = { B.start, B.start + (std::min)(block_size, B.length()) };
                            blockA.start += firstA.length();
                            RandomAccessIterator indexA = buffer1.start;

                            // if the first unevenly sized A block fits into the cache, move it there for when we go to Merge it
                            // otherwise, if the second buffer is available, block swap the contents into that
                            if (cache_size >  0 && lastA.length() <= cache_size) {
                                detail::move(lastA.start, lastA.end, cache_begin);
                            } else if (buffer2.length() > 0) {
                                detail::swap_ranges_overlap(lastA.start, lastA.end, buffer2.start);
                            }

                            if (blockA.length() > 0) {
                                while (true) {
                                    // if there's a previous B block and the first value of the minimum A block is <= the last value of the previous B block,
                                    // then drop that minimum A block behind. or if there are no B blocks left then keep dropping the remaining A blocks.
                                    if ((lastB.length() > 0 && not comp(proj(*std::prev(lastB.end)), proj(*indexA))) ||
                                        blockB.length() == 0) {
                                        // figure out where to split the previous B block, and rotate it at the split
                                        RandomAccessIterator B_split = lower_bound(lastB.start, lastB.end, proj(*indexA),
                                                                                   compare, projection);
                                        difference_type B_remaining = lastB.end - B_split;

                                        // swap the minimum A block to the beginning of the rolling A blocks
                                        RandomAccessIterator minA = blockA.start;
                                        for (auto findA = minA + block_size ; findA < blockA.end ; findA += block_size) {
                                            if (comp(proj(*findA), proj(*minA))) {
                                                minA = findA;
                                            }
                                        }
                                        detail::swap_ranges_overlap(blockA.start, blockA.start + block_size, minA);

                                        // swap the first item of the previous A block back with its original value, which is stored in buffer1
                                        iter_swap(blockA.start, indexA);
                                        ++indexA;

                                        // locally merge the previous A block with the B values that follow it
                                        // if lastA fits into the external cache we'll use it, else if the second
                                        // internal buffer exists we'll use it, otherwise we'll use a strictly
                                        // in-place merge algorithm
                                        if (cache_size > 0 && lastA.length() <= cache_size) {
                                            half_inplace_merge(cache_begin, cache_begin + lastA.length(),
                                                               lastA.end, B_split, lastA.start,
                                                               (std::min)(lastA.length(), B_split - lastA.end),
                                                               compare, projection);
                                        } else if (buffer2.length() > 0) {
                                            MergeInternal(lastA.start, lastA.end, lastA.end, B_split,
                                                          buffer2.start, compare, projection);
                                        } else {
                                            MergeInPlace(lastA.start, lastA.end, B_split, compare, projection);
                                        }

                                        if (buffer2.length() > 0 || (cache_size > 0 && block_size <= cache_size)) {
                                            // move the previous A block into the cache or buffer2, since
                                            // that's where we need it to be when we go to merge it anyway
                                            if (block_size <= cache_size) {
                                                detail::move(blockA.start, blockA.start + block_size, cache_begin);
                                                detail::move(B_split, B_split + B_remaining, blockA.start + (block_size - B_remaining));
                                            } else {
                                                detail::swap_ranges_overlap(blockA.start, blockA.start + block_size, buffer2.start);
                                                detail::swap_ranges_overlap(B_split, B_split + B_remaining, blockA.start + (block_size - B_remaining));
                                            }
                                        } else {
                                            // we are unable to use the 'buffer2' trick to speed up the rotation operation since buffer2 doesn't exist, so perform a normal rotation
                                            detail::rotate(B_split, blockA.start, blockA.start + block_size);
                                        }

                                        // update the range for the remaining A blocks, and the range remaining from the B block after it was split
                                        lastA = { blockA.start - B_remaining, blockA.start - (B_remaining - block_size) };
                                        lastB = { lastA.end, lastA.end + B_remaining };

                                        // if there are no more A blocks remaining, this step is finished!
                                        blockA.start += block_size;
                                        if (blockA.length() == 0) break;

                                    } else if (blockB.length() < block_size) {
                                        // move the last B block, which is unevenly sized, to before the remaining A blocks, by using a rotation
                                        detail::rotate(blockA.start, blockB.start, blockB.end);

                                        lastB = { blockA.start, blockA.start + blockB.length() };
                                        blockA.start += blockB.length();
                                        blockA.end += blockB.length();
                                        blockB.end = blockB.start;
                                    } else {
                                        // roll the leftmost A block to the end by swapping it with the next B block
                                        detail::swap_ranges_overlap(blockA.start, blockA.start + block_size, blockB.start);
                                        lastB = { blockA.start, blockA.start + block_size };

                                        blockA.start += block_size;
                                        blockA.end += block_size;
                                        blockB.start += block_size;

                                        if (blockB.end > B.end - block_size) {
                                            blockB.end = B.end;
                                        } else {
                                            blockB.end += block_size;
                                        }
                                    }
                                }
                            }

                            // merge the last A block with the remaining B values
                            if (cache_size > 0 && lastA.length() <= cache_size) {
                                half_inplace_merge(cache_begin, cache_begin + lastA.length(),
                                                   lastA.end, B.end, lastA.start,
                                                   (std::min)(lastA.length(), B.end - lastA.end),
                                                   compare, projection);
                            } else if (buffer2.length() > 0) {
                                MergeInternal(lastA.start, lastA.end, lastA.end, B.end,
                                              buffer2.start, compare, projection);
                            } else {
                                MergeInPlace(lastA.start, lastA.end, B.end, compare, projection);
                            }
                        }
                    }

                    // when we're finished with this merge step we should have the one or two internal buffers left over, where the second buffer is all jumbled up
                    // insertion sort the second buffer, then redistribute the buffers back into the array using the opposite process used for creating the buffer

                    // while an unstable sort like std::sort could be applied here, in benchmarks it was consistently slightly slower than a simple insertion sort,
                    // even for tens of millions of items. this may be because insertion sort is quite fast when the data is already somewhat sorted, like it is here
                    insertion_sort(buffer2.start, buffer2.end, compare, projection);

                    for (pull_index = 0 ; pull_index < 2 ; ++pull_index) {
                        difference_type unique = pull[pull_index].count * 2;
                        if (pull[pull_index].from > pull[pull_index].to) {
                            // the values were pulled out to the left, so redistribute them back to the right
                            Range<RandomAccessIterator> buffer = {
                                pull[pull_index].range.start,
                                pull[pull_index].range.start + pull[pull_index].count
                            };
                            while (buffer.length() > 0) {
                                index = FindFirstForward(buffer.end, pull[pull_index].range.end,
                                                         *buffer.start, compare, projection, unique);
                                difference_type amount = index - buffer.end;
                                detail::rotate(buffer.start, buffer.end, index);
                                buffer.start += (amount + 1);
                                buffer.end += amount;
                                unique -= 2;
                            }
                        } else if (pull[pull_index].from < pull[pull_index].to) {
                            // the values were pulled out to the right, so redistribute them back to the left
                            Range<RandomAccessIterator> buffer = {
                                pull[pull_index].range.end - pull[pull_index].count,
                                pull[pull_index].range.end
                            };
                            while (buffer.length() > 0) {
                                index = FindLastBackward(pull[pull_index].range.start, buffer.start,
                                                         *std::prev(buffer.end), compare, projection, unique);
                                difference_type amount = buffer.start - index;
                                detail::rotate(index, index + amount, buffer.end);
                                buffer.start -= amount;
                                buffer.end -= (amount + 1);
                                unique -= 2;
                            }
                        }
                    }
                }

                // double the size of each A and B subarray that will be merged in the next level
                if (not iterator.nextLevel()) break;
            }
        }
    }

    template<typename BufferProvider, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto wiki_sort(RandomAccessIterator first, RandomAccessIterator last,
                   Compare compare, Projection projection)
        -> void
    {
        // use a small cache to speed up some of the operations
        // just keep in mind that making it too small ruins the point (nothing will fit into it),
        // and making it too large also ruins the point (so much for "low memory"!)
        using rvalue_type = rvalue_type_t<RandomAccessIterator>;
        typename BufferProvider::template buffer<rvalue_type> cache(last - first);

        Wiki::sort(std::move(first), std::move(last),
                   cache.begin(), cache.size(),
                   std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_WIKI_SORT_H_
