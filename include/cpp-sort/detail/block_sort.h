/*
 * WikiSort: a public domain implementation of "Block Sort"
 * https://github.com/BonzaiThePenguin/WikiSort
 *
 * Modified in 2015 by Morwenn for inclusion into cpp-sort
 *
 */
#ifndef CPPSORT_DETAIL_BLOCK_SORT_H_
#define CPPSORT_DETAIL_BLOCK_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <limits>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/bitops.h>
#include "insertion_sort.h"
#include "lower_bound.h"
#include "merge.h"
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
            start(start),
            end(end)
        {}

        auto length() const
            -> std::size_t
        {
            return std::distance(start, end);
        }
    };

    // combine a linear search with a binary search to reduce the number of comparisons in situations
    // where have some idea as to how many unique values there are and where the next value might be
    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindFirstForward(RandomAccessIterator first, RandomAccessIterator last, const T & value,
                          Compare compare, Projection projection, std::size_t unique)
        -> RandomAccessIterator
    {
        std::size_t size = std::distance(first, last);
        if (size == 0) return first;

        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(value);
        std::size_t skip = std::max(size / unique, (std::size_t)1);

        RandomAccessIterator index;
        for (index = first + skip ; compare(proj(*std::prev(index)), value_proj) ; index += skip) {
            if (index >= last - skip) {
                return lower_bound(index, last, value_proj, compare, projection);
            }
        }
        return lower_bound(index - skip, index, value_proj, compare, projection);
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindLastForward(RandomAccessIterator first, RandomAccessIterator last, const T & value,
                         Compare compare, Projection projection, std::size_t unique)
        -> RandomAccessIterator
    {
        std::size_t size = std::distance(first, last);
        if (size == 0) return first;

        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(value);
        std::size_t skip = std::max(size / unique, (std::size_t)1);

        RandomAccessIterator index;
        for (index = first + skip ; not compare(value_proj, proj(*std::prev(index))) ; index += skip) {
            if (index >= last - skip) {
                return upper_bound(index, last, value_proj, compare, projection);
            }
        }
        return upper_bound(index - skip, index, value_proj, compare, projection);
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindFirstBackward(RandomAccessIterator first, RandomAccessIterator last, const T & value,
                           Compare compare, Projection projection, std::size_t unique)
        -> RandomAccessIterator
    {
        std::size_t size = std::distance(first, last);
        if (size == 0) return first;

        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(value);
        std::size_t skip = std::max(size / unique, (std::size_t)1);

        RandomAccessIterator index;
        for (index = last - skip ; index > first && not compare(proj(*std::prev(index)), value_proj) ; index -= skip) {
            if (index < first + skip) {
                return lower_bound(first, index, value_proj, compare, projection);
            }
        }
        return lower_bound(index, index + skip, value_proj, compare, projection);
    }

    template<typename RandomAccessIterator, typename T, typename Compare, typename Projection>
    auto FindLastBackward(RandomAccessIterator first, RandomAccessIterator last, const T & value,
                          Compare compare, Projection projection, std::size_t unique)
        -> RandomAccessIterator
    {
        std::size_t size = std::distance(first, last);
        if (size == 0) return first;

        auto&& proj = utility::as_function(projection);
        auto&& value_proj = proj(value);
        std::size_t skip = std::max(size / unique, (std::size_t)1);

        RandomAccessIterator index;
        for (index = last - skip ; index > first && compare(value_proj, proj(*std::prev(index))) ; index -= skip) {
            if (index < first + skip) {
                return upper_bound(first, index, value_proj, compare, projection);
            }
        }
        return upper_bound(index, index + skip, value_proj, compare, projection);
    }

    namespace Wiki
    {
        // merge operation using an external buffer
        template<typename RandomAccessIterator1, typename RandomAccessIterator2,
                 typename Compare, typename Projection>
        auto MergeExternal(RandomAccessIterator1 first1, RandomAccessIterator1 last1,
                           RandomAccessIterator1 first2, RandomAccessIterator1 last2,
                           RandomAccessIterator2 cache,
                           Compare compare, Projection projection)
            -> void
        {
            // A fits into the cache, so use that instead of the internal buffer
            RandomAccessIterator2 A_index = cache;
            RandomAccessIterator2 A_last = cache + std::distance(first1, last1);
            RandomAccessIterator1 B_index = first2;
            RandomAccessIterator1 B_last = last2;
            RandomAccessIterator1 insert_index = first1;

            auto&& proj = utility::as_function(projection);

            if (last2 - first2 > 0 && last1 - first1 > 0) {
                while (true) {
                    if (not compare(proj(*B_index), proj(*A_index))) {
                        *insert_index = *A_index;
                        ++A_index;
                        ++insert_index;
                        if (A_index == A_last) break;
                    } else {
                        *insert_index = *B_index;
                        ++B_index;
                        ++insert_index;
                        if (B_index == B_last) break;
                    }
                }
            }

            // copy the remainder of A into the final array
            std::copy(A_index, A_last, insert_index);
        }

        // merge operation using an internal buffer
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto MergeInternal(RandomAccessIterator first1, RandomAccessIterator last1,
                           RandomAccessIterator first2, RandomAccessIterator last2,
                           RandomAccessIterator buffer,
                           Compare compare, Projection projection)
            -> void
        {
            // whenever we find a value to add to the final array, swap it with the value that's already in that spot
            // when this algorithm is finished, 'buffer' will contain its original contents, but in a different order
            RandomAccessIterator A_index = buffer;
            RandomAccessIterator B_index = first2;
            RandomAccessIterator A_last = buffer + std::distance(first1, last1);
            RandomAccessIterator B_last = last2;
            RandomAccessIterator insert_index = first1;

            auto&& proj = utility::as_function(projection);

            if (last2 - first2 > 0 && last1 - first1 > 0) {
                while (true) {
                    if (not compare(proj(*B_index), proj(*A_index))) {
                        std::iter_swap(insert_index, A_index);
                        ++A_index;
                        ++insert_index;
                        if (A_index == A_last) break;
                    } else {
                        std::iter_swap(insert_index, B_index);
                        ++B_index;
                        ++insert_index;
                        if (B_index == B_last) break;
                    }
                }
            }

            // BlockSwap
            std::swap_ranges(A_index, A_last, insert_index);
        }

        // merge operation without a buffer
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto MergeInPlace(RandomAccessIterator first1, RandomAccessIterator last1,
                          RandomAccessIterator first2, RandomAccessIterator last2,
                          Compare compare, Projection projection)
            -> void
        {
            if (last1 - first1 == 0 || last2 - first2 == 0) return;

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
                RandomAccessIterator mid = lower_bound(first2, last2, proj(*first1), compare, projection);

                // rotate A into place
                std::size_t amount = mid - last1;
                std::rotate(first1, last1, mid);
                if (last2 == mid) break;

                // calculate the new A and B ranges
                first2 = mid;
                first1 += amount;
                last1 = first2;
                first1 = upper_bound(first1, last1, proj(*first1), compare, projection);
                if (std::distance(first1, last1) == 0) break;
            }
        }

        // calculate how to scale the index value to the range within the array
        // the bottom-up merge sort only operates on values that are powers of two,
        // so scale down to that power of two, then use a fraction to scale back again
        class Iterator
        {
            std::size_t size, power_of_two;
            std::size_t decimal, numerator, denominator;
            std::size_t decimal_step, numerator_step;

        public:

            Iterator(std::size_t size, std::size_t min_level):
                size(size),
                power_of_two(utility::hyperfloor(size)),
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

            template<typename Iterator>
            auto nextRange(Iterator it)
                -> Range<Iterator>
            {
                std::size_t start = decimal;

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
                -> std::size_t
            {
                return decimal_step;
            }
        };

        // bottom-up merge sort combined with an in-place merge algorithm for O(1) memory use
        template<typename BufferProvider, typename RandomAccessIterator,
                 typename Compare, typename Projection>
        auto sort(RandomAccessIterator first, RandomAccessIterator last,
                  Compare compare, Projection projection)
            -> void
        {
            using T = typename std::iterator_traits<RandomAccessIterator>::value_type;
            const std::size_t size = std::distance(first, last);
            auto&& proj = utility::as_function(projection);

            // if the array is of size 0, 1, 2, or 3, just sort them like so:
            if (size < 4) {
                if (size == 3) {
                    // hard-coded insertion sort
                    if (compare(proj(first[1]), proj(first[0]))) {
                        std::iter_swap(first + 0, first + 1);
                    }
                    if (compare(proj(first[2]), proj(first[1]))) {
                        std::iter_swap(first + 1, first + 2);
                        if (compare(proj(first[1]), proj(first[0]))) {
                            std::iter_swap(first + 0, first + 1);
                        }
                    }
                } else if (size == 2) {
                    // swap the items if they're out of order
                    if (compare(proj(first[1]), proj(first[0]))) {
                        std::iter_swap(first + 0, first + 1);
                    }
                }

                return;
            }

            // sort groups of 4-8 items at a time using an unstable sorting network,
            // but keep track of the original item orders to force it to be stable
            // http://pages.ripco.net/~jgamble/nw.html
            Wiki::Iterator iterator(size, 4);
            while (not iterator.finished()) {
                uint8_t order[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
                auto range = iterator.nextRange(first);

                #define SWAP(x, y) \
                    if (compare(proj(range.start[y]), proj(range.start[x])) || \
                        (order[x] > order[y] && not compare(proj(range.start[x]), proj(range.start[y])))) { \
                        std::iter_swap(range.start + x, range.start + y); \
                        std::iter_swap(order + x, order + y); }

                if (range.length() == 8) {
                    SWAP(0, 1); SWAP(2, 3); SWAP(4, 5); SWAP(6, 7);
                    SWAP(0, 2); SWAP(1, 3); SWAP(4, 6); SWAP(5, 7);
                    SWAP(1, 2); SWAP(5, 6); SWAP(0, 4); SWAP(3, 7);
                    SWAP(1, 5); SWAP(2, 6);
                    SWAP(1, 4); SWAP(3, 6);
                    SWAP(2, 4); SWAP(3, 5);
                    SWAP(3, 4);

                } else if (range.length() == 7) {
                    SWAP(1, 2); SWAP(3, 4); SWAP(5, 6);
                    SWAP(0, 2); SWAP(3, 5); SWAP(4, 6);
                    SWAP(0, 1); SWAP(4, 5); SWAP(2, 6);
                    SWAP(0, 4); SWAP(1, 5);
                    SWAP(0, 3); SWAP(2, 5);
                    SWAP(1, 3); SWAP(2, 4);
                    SWAP(2, 3);

                } else if (range.length() == 6) {
                    SWAP(1, 2); SWAP(4, 5);
                    SWAP(0, 2); SWAP(3, 5);
                    SWAP(0, 1); SWAP(3, 4); SWAP(2, 5);
                    SWAP(0, 3); SWAP(1, 4);
                    SWAP(2, 4); SWAP(1, 3);
                    SWAP(2, 3);

                } else if (range.length() == 5) {
                    SWAP(0, 1); SWAP(3, 4);
                    SWAP(2, 4);
                    SWAP(2, 3); SWAP(1, 4);
                    SWAP(0, 3);
                    SWAP(0, 2); SWAP(1, 3);
                    SWAP(1, 2);

                } else if (range.length() == 4) {
                    SWAP(0, 1); SWAP(2, 3);
                    SWAP(0, 2); SWAP(1, 3);
                    SWAP(1, 2);
                }

                #undef SWAP
            }
            if (size < 8) return;

            // use a small cache to speed up some of the operations
            // since the cache size is fixed, it's still O(1) memory!
            // just keep in mind that making it too small ruins the point (nothing will fit into it),
            // and making it too large also ruins the point (so much for "low memory"!)
            // removing the cache entirely still gives 75% of the performance of a standard merge
            typename BufferProvider::template buffer<T> cache(size);

            // then merge sort the higher levels, which can be 8-15, 16-31, 32-63, 64-127, etc.
            while (true) {
                // if every A and B block will fit into the cache, use a special branch specifically for merging with the cache
                // (we use < rather than <= since the block size might be one more than iterator.length())
                if (iterator.length() < cache.size()) {

                    // if four subarrays fit into the cache, it's faster to merge both pairs of subarrays into the cache,
                    // then merge the two merged subarrays from the cache back into the original array
                    if ((iterator.length() + 1) * 4 <= cache.size() && iterator.length() * 4 <= size) {
                        iterator.begin();
                        while (not iterator.finished()) {
                            // merge A1 and B1 into the cache
                            auto A1 = iterator.nextRange(first);
                            auto B1 = iterator.nextRange(first);
                            auto A2 = iterator.nextRange(first);
                            auto B2 = iterator.nextRange(first);

                            if (compare(proj(*std::prev(B1.end)), proj(*A1.start))) {
                                // the two ranges are in reverse order, so copy them in reverse order into the cache
                                std::copy(A1.start, A1.end, cache.begin() + B1.length());
                                std::copy(B1.start, B1.end, cache.begin());
                            } else if (compare(proj(*B1.start), proj(*std::prev(A1.end)))) {
                                // these two ranges weren't already in order, so merge them into the cache
                                merge(A1.start, A1.end, B1.start, B1.end, cache.begin(),
                                      compare, projection, projection);
                            } else {
                                // if A1, B1, A2, and B2 are all in order, skip doing anything else
                                if (not compare(proj(*B2.start), proj(*std::prev(A2.end))) &&
                                    not compare(proj(*A2.start), proj(*std::prev(B1.end)))) continue;

                                // copy A1 and B1 into the cache in the same order
                                std::copy(A1.start, A1.end, cache.begin());
                                std::copy(B1.start, B1.end, cache.begin() + A1.length());
                            }
                            A1 = { A1.start, B1.end };

                            // merge A2 and B2 into the cache
                            if (compare(proj(*std::prev(B2.end)), proj(*A2.start))) {
                                // the two ranges are in reverse order, so copy them in reverse order into the cache
                                std::copy(A2.start, A2.end, cache.begin() + A1.length() + B2.length());
                                std::copy(B2.start, B2.end, cache.begin() + A1.length());
                            } else if (compare(proj(*B2.start), proj(*std::prev(A2.end)))) {
                                // these two ranges weren't already in order, so merge them into the cache
                                merge(A2.start, A2.end, B2.start, B2.end, cache.begin() + A1.length(),
                                      compare, projection, projection);
                            } else {
                                // copy A2 and B2 into the cache in the same order
                                std::copy(A2.start, A2.end, cache.begin() + A1.length());
                                std::copy(B2.start, B2.end, cache.begin() + A1.length() + A2.length());
                            }
                            A2 = { A2.start, B2.end };

                            // merge A1 and A2 from the cache into the array
                            Range<T*> A3 = { cache.begin(), cache.begin() + A1.length() };
                            Range<T*> B3 = {
                                cache.begin() + A1.length(),
                                cache.begin() + A1.length() + A2.length()
                            };

                            if (compare(proj(*(B3.end - 1)), proj(*A3.start))) {
                                // the two ranges are in reverse order, so copy them in reverse order into the array
                                std::copy(A3.start, A3.end, A1.start + A2.length());
                                std::copy(B3.start, B3.end, A1.start);
                            } else if (compare(proj(*B3.start), proj(*std::prev(A3.end)))) {
                                // these two ranges weren't already in order, so merge them back into the array
                                merge(A3.start, A3.end, B3.start, B3.end, A1.start,
                                      compare, projection, projection);
                            } else {
                                // copy A3 and B3 into the array in the same order
                                std::copy(A3.start, A3.end, A1.start);
                                std::copy(B3.start, B3.end, A1.start + A1.length());
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

                            if (compare(proj(*std::prev(B.end)), proj(*A.start))) {
                                // the two ranges are in reverse order, so a simple rotation should fix it
                                std::rotate(A.start, A.end, B.end);
                            } else if (compare(proj(*B.start), proj(*std::prev(A.end)))) {
                                // these two ranges weren't already in order, so we'll need to merge them!
                                std::copy(A.start, A.end, cache.begin());
                                MergeExternal(A.start, A.end, B.start, B.end,
                                              cache.begin(), compare, projection);
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

                    std::size_t block_size = std::sqrt(iterator.length());
                    std::size_t buffer_size = iterator.length() / block_size + 1;

                    // as an optimization, we really only need to pull out the internal buffers once for each level of merges
                    // after that we can reuse the same buffers over and over, then redistribute it when we're finished with this level
                    Range<RandomAccessIterator> buffer1 = { first, first };
                    Range<RandomAccessIterator> buffer2 = { first, first };
                    RandomAccessIterator index, last;
                    std::size_t count, pull_index = 0;
                    struct
                    {
                        RandomAccessIterator from, to;
                        std::size_t count;
                        Range<RandomAccessIterator> range;
                    } pull[2];
                    pull[0].count = 0; pull[0].range = { first, first };
                    pull[1].count = 0; pull[1].range = { first, first };

                    // find two internal buffers of size 'buffer_size' each
                    // let's try finding both buffers at the same time from a single A or B subarray
                    std::size_t find = buffer_size + buffer_size;
                    bool find_separately = false;

                    if (block_size <= cache.size()) {
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
                        #define PULL(_to) \
                            pull[pull_index].range = { A.start, B.end }; \
                            pull[pull_index].count = count; \
                            pull[pull_index].from = index; \
                            pull[pull_index].to = _to

                        // check A for the number of unique values we need to fill an internal buffer
                        // these values will be pulled out to the start of A
                        for (last = A.start, count = 1; count < find; last = index, ++count) {
                            index = FindLastForward(std::next(last), A.end, *last, compare, projection, find - count);
                            if (index == A.end) break;
                            assert(index < A.end);
                        }
                        index = last;

                        if (count >= buffer_size) {
                            // keep track of the range within the array where we'll need to "pull out" these values to create the internal buffer
                            PULL(A.start);
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
                            } else if (block_size <= cache.size()) {
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
                            PULL(A.start);
                        }

                        // check B for the number of unique values we need to fill an internal buffer
                        // these values will be pulled out to the end of B
                        for (last = std::prev(B.end), count = 1; count < find; last = index - 1, ++count) {
                            index = FindFirstBackward(B.start, last, *last, compare, projection, find - count);
                            if (index == B.start) break;
                            assert(index > B.start);
                        }
                        index = last;

                        if (count >= buffer_size) {
                            // keep track of the range within the array where we'll need to "pull out" these values to create the internal buffer
                            PULL(B.end);
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
                            } else if (block_size <= cache.size()) {
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
                            PULL(B.end);
                        }

                        #undef PULL
                    }

                    // pull out the two ranges so we can use them as internal buffers
                    for (pull_index = 0; pull_index < 2; ++pull_index) {
                        std::size_t length = pull[pull_index].count;

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
                                std::rotate(range.start, range.end - count, range.end);
                                pull[pull_index].from = index + count;
                            }
                        } else if (pull[pull_index].to > pull[pull_index].from) {
                            // we're pulling values out to the right, which means the end of a B subarray
                            index = std::next(pull[pull_index].from);
                            for (count = 1; count < length; ++count) {
                                index = FindLastForward(index, pull[pull_index].to, *index,
                                                        compare, projection, length - count);
                                Range<RandomAccessIterator> range = { pull[pull_index].from, std::prev(index) };
                                std::rotate(range.start, range.start + count, range.end);
                                pull[pull_index].from = std::prev(index - count);
                            }
                        }
                    }

                    // adjust block_size and buffer_size based on the values we were able to pull out
                    buffer_size = buffer1.length();
                    block_size = iterator.length() / buffer_size + 1;

                    // the first buffer NEEDS to be large enough to tag each of the evenly sized A blocks,
                    // so this was originally here to test the math for adjusting block_size above
                    //assert((iterator.length() + 1)/block_size <= buffer_size);

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
                                // which also only happens when cache size is small or 0 since it'd otherwise use MergeExternal
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

                        if (compare(proj(*std::prev(B.end)), proj(*A.start))) {
                            // the two ranges are in reverse order, so a simple rotation should fix it
                            std::rotate(A.start, A.end, B.end);
                        } else if (compare(proj(*A.end), proj(*std::prev(A.end)))) {
                            // these two ranges weren't already in order, so we'll need to merge them!

                            // break the remainder of A into blocks. firstA is the uneven-sized first A block
                            Range<RandomAccessIterator> blockA = A;
                            Range<RandomAccessIterator> firstA = { A.start, A.start + blockA.length() % block_size };

                            // swap the first value of each A block with the values in buffer1
                            for (auto indexA = buffer1.start, index = firstA.end ; index < blockA.end ; ++indexA, index += block_size) {
                                std::iter_swap(indexA, index);
                            }

                            // start rolling the A blocks through the B blocks!
                            // when we leave an A block behind we'll need to merge the previous A block with any B blocks that follow it, so track that information as well
                            Range<RandomAccessIterator> lastA = firstA;
                            Range<RandomAccessIterator> lastB = { first, first };
                            Range<RandomAccessIterator> blockB = { B.start, B.start + std::min(block_size, B.length()) };
                            blockA.start += firstA.length();
                            RandomAccessIterator indexA = buffer1.start;

                            // if the first unevenly sized A block fits into the cache, copy it there for when we go to Merge it
                            // otherwise, if the second buffer is available, block swap the contents into that
                            if (lastA.length() <= cache.size()) {
                                std::copy(lastA.start, lastA.end, cache.begin());
                            } else if (buffer2.length() > 0) {
                                std::swap_ranges(lastA.start, lastA.end, buffer2.start);
                            }

                            if (blockA.length() > 0) {
                                while (true) {
                                    // if there's a previous B block and the first value of the minimum A block is <= the last value of the previous B block,
                                    // then drop that minimum A block behind. or if there are no B blocks left then keep dropping the remaining A blocks.
                                    if ((lastB.length() > 0 && not compare(proj(*std::prev(lastB.end)), proj(*indexA))) ||
                                        blockB.length() == 0) {
                                        // figure out where to split the previous B block, and rotate it at the split
                                        RandomAccessIterator B_split = lower_bound(lastB.start, lastB.end, proj(*indexA),
                                                                                   compare, projection);
                                        std::size_t B_remaining = std::distance(B_split, lastB.end);

                                        // swap the minimum A block to the beginning of the rolling A blocks
                                        RandomAccessIterator minA = blockA.start;
                                        for (auto findA = minA + block_size ; findA < blockA.end ; findA += block_size) {
                                            if (compare(proj(*findA), proj(*minA))) {
                                                minA = findA;
                                            }
                                        }
                                        std::swap_ranges(blockA.start, blockA.start + block_size, minA);

                                        // swap the first item of the previous A block back with its original value, which is stored in buffer1
                                        std::iter_swap(blockA.start, indexA);
                                        ++indexA;

                                        // locally merge the previous A block with the B values that follow it
                                        // if lastA fits into the external cache we'll use that (with MergeExternal),
                                        // or if the second internal buffer exists we'll use that (with MergeInternal),
                                        // or failing that we'll use a strictly in-place merge algorithm (MergeInPlace)
                                        if (lastA.length() <= cache.size()) {
                                            MergeExternal(lastA.start, lastA.end, lastA.end, B_split,
                                                          cache.begin(), compare, projection);
                                        } else if (buffer2.length() > 0) {
                                            MergeInternal(lastA.start, lastA.end, lastA.end, B_split,
                                                          buffer2.start, compare, projection);
                                        } else {
                                            MergeInPlace(lastA.start, lastA.end, lastA.end, B_split, compare, projection);
                                        }

                                        if (buffer2.length() > 0 || block_size <= cache.size()) {
                                            // copy the previous A block into the cache or buffer2, since that's where we need it to be when we go to merge it anyway
                                            if (block_size <= cache.size()) {
                                                std::copy(blockA.start, blockA.start + block_size, cache.begin());
                                            } else {
                                                std::swap_ranges(blockA.start, blockA.start + block_size, buffer2.start);
                                            }

                                            // this is equivalent to rotating, but faster
                                            // the area normally taken up by the A block is either the contents of buffer2, or data we don't need anymore since we memcopied it
                                            // either way we don't need to retain the order of those items, so instead of rotating we can just block swap B to where it belongs
                                            std::swap_ranges(B_split, B_split + B_remaining, blockA.start + block_size - B_remaining);
                                        } else {
                                            // we are unable to use the 'buffer2' trick to speed up the rotation operation since buffer2 doesn't exist, so perform a normal rotation
                                            std::rotate(B_split, blockA.start, blockA.start + block_size);
                                        }

                                        // update the range for the remaining A blocks, and the range remaining from the B block after it was split
                                        lastA = { blockA.start - B_remaining, blockA.start - B_remaining + block_size };
                                        lastB = { lastA.end, lastA.end + B_remaining };

                                        // if there are no more A blocks remaining, this step is finished!
                                        blockA.start += block_size;
                                        if (blockA.length() == 0) break;

                                    } else if (blockB.length() < block_size) {
                                        // move the last B block, which is unevenly sized, to before the remaining A blocks, by using a rotation
                                        std::rotate(blockA.start, blockB.start, blockB.end);

                                        lastB = { blockA.start, blockA.start + blockB.length() };
                                        blockA.start += blockB.length();
                                        blockA.end += blockB.length();
                                        blockB.end = blockB.start;
                                    } else {
                                        // roll the leftmost A block to the end by swapping it with the next B block
                                        std::swap_ranges(blockA.start, blockA.start + block_size, blockB.start);
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
                            if (lastA.length() <= cache.size()) {
                                MergeExternal(lastA.start, lastA.end, lastA.end, B.end,
                                              cache.begin(), compare, projection);
                            } else if (buffer2.length() > 0) {
                                MergeInternal(lastA.start, lastA.end, lastA.end, B.end,
                                              buffer2.start, compare, projection);
                            } else {
                                MergeInPlace(lastA.start, lastA.end, lastA.end, B.end, compare, projection);
                            }
                        }
                    }

                    // when we're finished with this merge step we should have the one or two internal buffers left over, where the second buffer is all jumbled up
                    // insertion sort the second buffer, then redistribute the buffers back into the array using the opposite process used for creating the buffer

                    // while an unstable sort like std::sort could be applied here, in benchmarks it was consistently slightly slower than a simple insertion sort,
                    // even for tens of millions of items. this may be because insertion sort is quite fast when the data is already somewhat sorted, like it is here
                    insertion_sort(buffer2.start, buffer2.end, compare, projection);

                    for (pull_index = 0 ; pull_index < 2 ; ++pull_index) {
                        std::size_t unique = pull[pull_index].count * 2;
                        if (pull[pull_index].from > pull[pull_index].to) {
                            // the values were pulled out to the left, so redistribute them back to the right
                            Range<RandomAccessIterator> buffer = {
                                pull[pull_index].range.start,
                                pull[pull_index].range.start + pull[pull_index].count
                            };
                            while (buffer.length() > 0) {
                                index = FindFirstForward(buffer.end, pull[pull_index].range.end,
                                                         *buffer.start, compare, projection, unique);
                                std::size_t amount = index - buffer.end;
                                std::rotate(buffer.start, buffer.end, index);
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
                                std::size_t amount = buffer.start - index;
                                std::rotate(index, index + amount, buffer.end);
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
    auto block_sort(RandomAccessIterator first, RandomAccessIterator last,
                    Compare compare, Projection projection)
        -> void
    {
        Wiki::sort<BufferProvider>(first, last, compare, projection);
    }
}}

#endif // CPPSORT_DETAIL_BLOCK_SORT_H_
