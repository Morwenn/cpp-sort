/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*          Copyright Andrei Alexandrescu, 2016-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
/*          Copyright Danila Kutenin, 2020-.
 * Distributed under the Boost Software License, Version 1.0.
 *    (See accompanying file LICENSE_1_0.txt or copy at
 *          https://boost.org/LICENSE_1_0.txt)
 */
#ifndef CPPSORT_DETAIL_QUICKSELECT_ADAPTIVE_H_
#define CPPSORT_DETAIL_QUICKSELECT_ADAPTIVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "config.h"
#include "partition.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    namespace median_common_detail
    {
        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto pivot_partition(RandomAccessIterator r, difference_type_t<RandomAccessIterator> k,
                             difference_type_t<RandomAccessIterator> length,
                             Compare compare, Projection projection)
            -> RandomAccessIterator
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            CPPSORT_ASSERT(k < length);
            iter_swap(r, r + k); // Move pivot out of the way
            auto&& pivot_proj = proj(*r);
            auto pivot_pos = detail::partition(r, r + length, [&](auto&& elem) {
                return not comp(pivot_proj, proj(elem));
            });
            iter_swap(r, --pivot_pos); // Put pivot back in its place
            return pivot_pos;
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto median_index(RandomAccessIterator r, difference_type_t<RandomAccessIterator> a,
                          difference_type_t<RandomAccessIterator> b, difference_type_t<RandomAccessIterator> c,
                          Compare compare, Projection projection)
            -> difference_type_t<RandomAccessIterator>
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (comp(proj(r[c]), proj(r[a]))) {
                std::swap(a, c);
            }
            if (comp(proj(r[c]), proj(r[b]))) {
                return c;
            }
            if (comp(proj(r[b]), proj(r[a]))) {
                return a;
            }
            return b;
        }

        template<bool LeanRight, typename RandomAccessIterator, typename Compare, typename Projection>
        auto median_index(RandomAccessIterator r,
                          difference_type_t<RandomAccessIterator> a, difference_type_t<RandomAccessIterator> b,
                          difference_type_t<RandomAccessIterator> c, difference_type_t<RandomAccessIterator> d,
                          Compare compare, Projection projection)
            -> difference_type_t<RandomAccessIterator>
        {
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (comp(proj(r[d]), proj(r[c]))) {
                std::swap(c, d);
            }

            if (LeanRight) {
                if (comp(proj(r[c]), proj(r[a]))) {
                    CPPSORT_ASSERT(comp(proj(r[c]), proj(r[a])) && not comp(proj(r[d]), proj(r[c])));  // so r[c]) is out
                    return median_index(r, a, b, d, std::move(compare), std::move(projection));
                }
            } else {
                if (not comp(proj(r[d]), proj(r[a]))) {
                    return median_index(r, a, b, c, std::move(compare), std::move(projection));
                }
            }

            // Could return median_index(r, b, c, d) but we already know r[c] <= r[d]
            if (not comp(proj(r[c]), proj(r[b]))) {
                return c;
            }
            if (comp(proj(r[d]), proj(r[b]))) {
                return d;
            }
            return b;
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto ninther(RandomAccessIterator r, difference_type_t<RandomAccessIterator> _1, difference_type_t<RandomAccessIterator> _2,
                     difference_type_t<RandomAccessIterator> _3, difference_type_t<RandomAccessIterator> _4,
                     difference_type_t<RandomAccessIterator> _5, difference_type_t<RandomAccessIterator> _6,
                     difference_type_t<RandomAccessIterator> _7, difference_type_t<RandomAccessIterator> _8,
                     difference_type_t<RandomAccessIterator> _9, Compare compare, Projection projection)
            -> void
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            _2 = median_index(r, _1, _2, _3, compare, projection);
            _8 = median_index(r, _7, _8, _9, compare, projection);
            if (comp(proj(r[_8]), proj(r[_2]))) {
                std::swap(_2, _8);
            }
            if (comp(proj(r[_6]), proj(r[_4]))) {
                std::swap(_4, _6);
            }

            // Here we know that r[_2] and r[_8] are the other two medians and that
            // r[_2] <= r[_8]. We also know that r[_4] <= r[_6]
            if (comp(proj(r[_5]), proj(r[_4]))) {
                // r[_4] is the median of r[_4], r[_5], r[_6]
            } else if (comp(proj(r[_6]), proj(r[_5]))) {
                // r[_6] is the median of r[_4], r[_5], r[_6]
                _4 = _6;
            } else {
                // Here we know r[_5] is the median of r[_4], r[_5], r[_6]
                if (comp(proj(r[_5]), proj(r[_2]))) {
                    iter_swap(r + _5, r + _2);
                    return;
                }
                if (comp(proj(r[_8]), proj(r[_5]))) {
                    iter_swap(r + _5, r + _8);
                    return;
                }
                // This is the only path that returns with no swap
                return;
            }

            // Here we know r[_4] is the median of r[_4], r[_5], r[_6]
            if (comp(proj(r[_4]), proj(r[_2]))) {
                _4 = _2;
            } else if (comp(proj(r[_8]), proj(r[_4]))) {
                _4 = _8;
            }
            iter_swap(r + _5, r + _4);
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto expand_partition_left(RandomAccessIterator r, difference_type_t<RandomAccessIterator> lo,
                                   difference_type_t<RandomAccessIterator> pivot,
                                   Compare compare, Projection projection)
            -> difference_type_t<RandomAccessIterator>
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            CPPSORT_ASSERT(lo > 0 && lo <= pivot);
            difference_type_t<RandomAccessIterator> left = 0;
            const auto oldPivot = pivot;
            auto&& old_pivot_proj = proj(r[oldPivot]);
            for (; lo < pivot ; ++left) {
                if (left == lo) {
                    iter_swap(r + oldPivot, r + pivot);
                    return pivot;
                }
                if (not comp(old_pivot_proj, proj(r[left]))) continue;
                --pivot;
                iter_swap(r + left, r + pivot);
            }

            // Second loop: make left and pivot meet
            for (;; ++left) {
                if (left == pivot) break;
                if (not comp(old_pivot_proj, proj(r[left]))) continue;
                for (;;) {
                    if (left == pivot) {
                        iter_swap(r + oldPivot, r + pivot);
                        return pivot;
                    }
                    --pivot;
                    if (comp(proj(r[pivot]), old_pivot_proj)) {
                        iter_swap(r + left, r + pivot);
                        break;
                    }
                }
            }

            iter_swap(r + oldPivot, r + pivot);
            return pivot;
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto expand_partition_right(RandomAccessIterator r, difference_type_t<RandomAccessIterator> hi,
                                    difference_type_t<RandomAccessIterator> rite,
                                    Compare compare, Projection projection)
            -> difference_type_t<RandomAccessIterator>
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            difference_type_t<RandomAccessIterator> pivot = 0;
            CPPSORT_ASSERT(pivot <= hi);
            CPPSORT_ASSERT(hi <= rite);
            auto&& pivot_proj = proj(r[0]);

            // First loop: spend r[pivot .. hi]
            for (; pivot < hi ; --rite) {
                if (rite == hi) {
                    iter_swap(r, r + pivot);
                    return pivot;
                }
                if (not comp(proj(r[rite]), pivot_proj)) continue;
                ++pivot;
                iter_swap(r + rite, r + pivot);
            }

            // Second loop: make left and pivot meet
            for (; rite > pivot ; --rite) {
                if (not comp(proj(r[rite]), pivot_proj)) continue;
                while (rite > pivot) {
                    ++pivot;
                    if (comp(pivot_proj, proj(r[pivot]))) {
                        iter_swap(r + rite, r + pivot);
                        break;
                    }
                }
            }

            iter_swap(r, r + pivot);
            return pivot;
        }

        template<typename RandomAccessIterator, typename Compare, typename Projection>
        auto expand_partition(RandomAccessIterator r, difference_type_t<RandomAccessIterator> lo,
                              difference_type_t<RandomAccessIterator> pivot, difference_type_t<RandomAccessIterator> hi,
                              difference_type_t<RandomAccessIterator> length, Compare compare, Projection projection)
            -> difference_type_t<RandomAccessIterator>
        {
            using utility::iter_swap;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            CPPSORT_ASSERT(lo <= pivot && pivot < hi && hi <= length);
            --hi;
            --length;

            for (difference_type_t<RandomAccessIterator> left = 0 ; ; ++left, --length) {
                for (;; ++left) {
                    if (left == lo) {
                        return pivot + expand_partition_right(r + pivot, hi - pivot, length - pivot,
                                                              std::move(compare), std::move(projection));
                    }
                    if (comp(proj(r[pivot]), proj(r[left]))) break;
                }
                for (;; --length) {
                    if (length == hi) {
                        return left + expand_partition_left(r + left, lo - left, pivot - left,
                                                            std::move(compare), std::move(projection));
                    }
                    if (not comp(proj(r[pivot]), proj(r[length]))) break;
                }
                iter_swap(r + left, r + length);
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto adaptive_quickselect(RandomAccessIterator r, difference_type_t<RandomAccessIterator> n,
                              difference_type_t<RandomAccessIterator> length,
                              Compare compare, Projection projection)
        -> void;

    /*
    Median of minima
    */
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto median_of_minima(RandomAccessIterator r, difference_type_t<RandomAccessIterator> n,
                          difference_type_t<RandomAccessIterator> length,
                          Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        CPPSORT_ASSERT(length >= 2);
        CPPSORT_ASSERT(n <= length / 6);
        CPPSORT_ASSERT(n > 0);

        auto subset = n * 2;
        auto computeMinOver = (length - subset) / subset;
        CPPSORT_ASSERT(computeMinOver > 0);

        for (difference_type_t<RandomAccessIterator> i = 0, j = subset ; i < subset ; ++i) {
            auto limit = j + computeMinOver;
            auto minIndex = j;
            while (++j < limit) {
                if (comp(proj(r[j]), proj(r[minIndex]))) {
                    minIndex = j;
                }
            }
            if (comp(proj(r[minIndex]), proj(r[i]))) {
                using utility::iter_swap;
                iter_swap(r + i, r + minIndex);
            }
            CPPSORT_ASSERT(j < length || i + 1 == subset);
        }

        adaptive_quickselect(r, n, subset, compare, projection);
        return median_common_detail::expand_partition(r, 0, n, subset, length,
                                                      std::move(compare), std::move(projection));
    }

    /*
    Median of maxima
    */
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto median_of_maxima(RandomAccessIterator r, difference_type_t<RandomAccessIterator> n,
                          difference_type_t<RandomAccessIterator> length,
                          Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        using utility::iter_swap;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        CPPSORT_ASSERT(length >= 2);
        CPPSORT_ASSERT(n < length && n / 5 >= length - n);

        auto subset = (length - n) * 2;
        auto subsetStart = length - subset;
        auto computeMaxOver = subsetStart / subset;
        CPPSORT_ASSERT(computeMaxOver > 0);

        for (auto i = subsetStart, j = i - subset * computeMaxOver ; i < length ; ++i) {
            auto limit = j + computeMaxOver;
            auto maxIndex = j;
            while (++j < limit) {
                if (comp(proj(r[maxIndex]), proj(r[j]))) {
                    maxIndex = j;
                }
            }
            if (comp(proj(r[i]), proj(r[maxIndex]))) {
                iter_swap(r + i, r + maxIndex);
            }
            CPPSORT_ASSERT(j != 0 || i + 1 == length);
        }

        adaptive_quickselect(r + subsetStart, length - n, subset, compare, projection);
        return median_common_detail::expand_partition(r, subsetStart, n, length, length,
                                                      std::move(compare), std::move(projection));
    }

    /*
    Partitions r[0 .. length] using a pivot of its own choosing. Attempts to pick a
    pivot that approximates the median. Returns the position of the pivot.
    */
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto median_of_ninthers(RandomAccessIterator r, difference_type_t<RandomAccessIterator> length,
                            Compare compare, Projection projection)
        -> difference_type_t<RandomAccessIterator>
    {
        CPPSORT_ASSERT(length >= 12);
        auto frac = length <= 1024 ? length / 12
                    : length <= 128 * 1024 ? length / 64
                    : length / 1024;
        auto pivot = frac / 2;
        auto lo = length / 2 - pivot;
        auto hi = lo + frac;
        CPPSORT_ASSERT(lo >= frac * 4);
        CPPSORT_ASSERT(length - hi >= frac * 4);
        CPPSORT_ASSERT(lo / 2 >= pivot);

        auto gap = (length - 9 * frac) / 4;
        auto a = lo - 4 * frac - gap;
        auto b = hi + gap;
        for (auto i = lo ; i < hi ; ++i, a += 3, b += 3) {
            median_common_detail::ninther(r,
                a, i - frac, b,
                a + 1, i, b + 1,
                a + 2, i + frac, b + 2,
                compare, projection
            );
        }

        adaptive_quickselect(r + lo, pivot, frac, compare, projection);
        return median_common_detail::expand_partition(
            r, lo, lo + pivot, hi, length,
            std::move(compare), std::move(projection)
        );
    }

    /*
    Quickselect driver for median_of_ninthers, median_of_minima, and
    median_of_maxima. Dispathes to each depending on the relationship between n (the
    sought order statistics) and length.
    */
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto adaptive_quickselect(RandomAccessIterator r, difference_type_t<RandomAccessIterator> n,
                              difference_type_t<RandomAccessIterator> length,
                              Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        CPPSORT_ASSERT(n < length);
        for (;;) {
            // Decide strategy for partitioning
            if (n == 0) {
                // That would be the max
                difference_type_t<RandomAccessIterator> pivot = n;
                for (++n ; n < length ; ++n) {
                    if (comp(proj(r[n]), proj(r[pivot]))) {
                        pivot = n;
                    }
                }
                iter_swap(r, r + pivot);
                return;
            }

            if (n + 1 == length) {
                // That would be the min
                difference_type_t<RandomAccessIterator> pivot = 0;
                for (n = 1 ; n < length ; ++n) {
                    if (comp(proj(r[pivot]), proj(r[n]))) {
                        pivot = n;
                    }
                }
                iter_swap(r + pivot, r + (length - 1));
                return;
            }

            CPPSORT_ASSERT(n < length);
            difference_type_t<RandomAccessIterator> pivot;
            if (length <= 16) {
                pivot = median_common_detail::pivot_partition(r, n, length, compare, projection) - r;
            } else if (n <= length / 6) {
                pivot = median_of_minima(r, n, length, compare, projection);
            } else if (n / 5 >= length - n) {
                pivot = median_of_maxima(r, n, length, compare, projection);
            } else {
                pivot = median_of_ninthers(r, length, compare, projection);
            }

            // See how the pivot fares
            if (pivot == n) return;
            if (pivot > n) {
                length = pivot;
            } else {
                ++pivot;
                r += pivot;
                length -= pivot;
                n -= pivot;
            }
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto median_of_ninthers_select(RandomAccessIterator begin, RandomAccessIterator mid, RandomAccessIterator end,
                                   Compare compare, Projection projection)
        -> RandomAccessIterator
    {
        if (mid != end) {
            adaptive_quickselect(begin, mid - begin, end - begin, compare, projection);
        }
        return mid;
    }
}}

#endif // CPPSORT_DETAIL_QUICKSELECT_ADAPTIVE_H_
