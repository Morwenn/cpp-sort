/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_VERGESORT_H_
#define CPPSORT_DETAIL_VERGESORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <list>
#include <utility>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"
#include "config.h"
#include "inplace_merge.h"
#include "iterator_traits.h"
#include "lower_bound.h"
#include "quick_merge_sort.h"
#include "reverse.h"
#include "rotate.h"
#include "sized_iterator.h"
#include "upper_bound.h"

namespace cppsort
{
namespace detail
{
namespace verge
{
    ////////////////////////////////////////////////////////////
    // Run helper class, represents a non-decreasing run with
    // its size and end iterator

    template<typename Iterator>
    struct run
    {
        Iterator end;
        difference_type_t<Iterator> size;
    };

    ////////////////////////////////////////////////////////////
    // Merge a list of runs with a k-way merge

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_runs(BidirectionalIterator first, std::list<verge::run<BidirectionalIterator>>& runs,
                    Compare compare, Projection projection)
        -> void
    {
        if (runs.size() < 2) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Merge runs pairwise until there are no runs left
        do {
            auto begin = first;
            for (auto it = runs.begin() ; it != runs.end() && it != std::prev(runs.end()) ; ++it) {
                auto next_it = std::next(it);

                // Merge the runs, micro-optimize for size 1 because it can happen,
                // and the generic inplace_merge algorithm cares not
                if (it->size == 1) {
                    auto&& target = proj(*begin);
                    if (comp(proj(*it->end), target)) {
                        auto insert_it = detail::lower_bound_n(it->end, next_it->size, target, compare, projection);
                        detail::rotate_left(first, insert_it);
                    } else {
                    }
                } else if (next_it->size == 1) {
                    auto&& target = proj(*std::prev(next_it->end));
                    if (comp(target, proj(*std::prev(it->end)))) {
                        auto insert_it = detail::upper_bound_n(begin, it->size, target, compare, projection);
                        detail::rotate_right(insert_it, next_it->end);
                    }
                } else {
                    detail::inplace_merge(begin, it->end, next_it->end, compare, projection,
                                          it->size, next_it->size);
                }

                // Compute the size of the new merged run
                next_it->size += it->size;
                // Remove the middle iterator to order to fuse the two
                // consecutive runs, and advance to the next pair
                it = runs.erase(it);
                begin = it->end;
            }
        } while (runs.size() > 1);
    }

    ////////////////////////////////////////////////////////////
    // Vergesort main algorithms

    template<
        bool Stable,
        typename BidirectionalIterator,
        typename Compare,
        typename Projection,
        typename Fallback
    >
    auto sort(std::bidirectional_iterator_tag,
              BidirectionalIterator first, BidirectionalIterator last,
              difference_type_t<BidirectionalIterator> size,
              Compare compare, Projection projection, Fallback fallback)
        -> void
    {
        if (size < 128) {
            // vergesort is inefficient for small collections
            fallback(make_sized_iterator(first, size),
                     make_sized_iterator(last, size),
                     std::move(compare), std::move(projection));
            return;
        }

        using difference_type = difference_type_t<BidirectionalIterator>;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Limit under which fallback is used
        int minrun_limit = size / log2(size);

        // Vergesort detects big runs in ascending or descending order,
        // and remembers where each run ends by storing the end iterator
        // of each run in this list, then it performs a k-way merge
        std::list<verge::run<BidirectionalIterator>> runs;

        // Beginning of an "unsorted" partition, last if the previous
        // partition is sorted: as long as the algorithm does not find a
        // big enough run, it "accumulates" contiguous unsorted elements,
        // and sorts them all at once when it reaches the end of such a
        // partition, to ensure that a minimal number of calls to the
        // fallback algorithm are performed
        auto begin_unsorted = last;

        // Size of the current unsorted partition
        difference_type size_unsorted = 0;

        // Pair of iterators to iterate through the collection
        auto current = first;
        auto next = std::next(current);

        // Choose whether to look for an ascending or descending
        // run depending on the value of the first two elements;
        // when comparing equivalent there is a bias towards
        // ascending runs because they don't have to be reversed,
        // and is always the right thing to do when sorting stably
        if (comp(proj(*next), proj(*current))) {
            goto find_descending;
        } else {
            goto find_ascending;
        }

        while (true) {

            // This main loop uses goto here and there, but it is written in such
            // a way that gotos are not part of the hot path: there are O(log n)
            // gotos performed while the loop itself is O(n).
            //
            // The job of this loop is to find ascending and descending runs that
            // are big enough according to vergesort's heuristic. In order to do
            // that we perform a linear scan with the following intuition: when we
            // reach two elements whose order does not match that of the run we are
            // currently trying to identify, we know that we have reached the limit
            // of the current run. When it happens, there are two possible scenarios:
            // - The run we found is big enough, we add it whole to the list of runs
            //   to merge later. We take the next two elements and compare them to
            //   decide whether we should start looking for an ascending or for a
            //   descending run next.
            // - The run we found is not big enough, so we consider the last *two*
            //   elements to be potentially part of the next run. We then start
            //   looking for a run whose direction matches that of the two elements
            //   we just compared.
            //
            // The result is that the hot path it the one that constantly fails to
            // find big enough runs, and it naturally performs a kind of ping-pong
            // between looking for ascending and descending runs. So this loop is
            // optimized for this scenario: when we start looking for a run we
            // already know the order of the first two elements. However when we
            // find a big enough run we only consider the last compared element to
            // be part of the next run, and not the last two like we do in the hot
            // path. This break the ping-pong pattern, so we use goto to jump directly
            // to the run detection code corresponding to the order of the new two
            // elements.

            find_ascending: {
                CPPSORT_ASSERT(not comp(proj(*next), proj(*current)));

                auto begin_rng = current;
                ++current;
                ++next;

                // Find a non-descending run
                difference_type run_size = 2;
                while (next != last) {
                    if (comp(proj(*next), proj(*current))) break;
                    ++current;
                    ++next;
                    ++run_size;
                }

                if (run_size > minrun_limit) {
                    if (begin_unsorted != last) {
                        fallback(make_sized_iterator(begin_unsorted, size_unsorted),
                                 make_sized_iterator(begin_rng, size_unsorted),
                                 compare, projection);
                        runs.push_back({ begin_rng, size_unsorted} );
                        runs.push_back({ next, run_size });
                        begin_unsorted = last;
                        size_unsorted = 0;
                    } else {
                        runs.push_back({ next, run_size });
                    }
                    if (next == last) break;

                    // Find and start a new run
                    ++current;
                    ++next;
                    if (next == last) {
                        begin_unsorted = current;
                        size_unsorted = 0;
                        break;
                    }
                    if (comp(proj(*next), proj(*current))) {
                        goto find_descending;
                    } else {
                        goto find_ascending;
                    }
                } else {
                    size_unsorted += (run_size - 1);
                    if (begin_unsorted == last) {
                        begin_unsorted = begin_rng;
                    }
                }

                if (next == last) break;
            }

            find_descending: {
                CPPSORT_ASSERT(comp(proj(*next), proj(*current)));

                auto begin_rng = current;
                ++current;
                ++next;

                difference_type run_size = 2;
                if (Stable) {
                    // Find a strictly descending run
                    while (next != last) {
                        if (not comp(proj(*next), proj(*current))) break;
                        ++current;
                        ++next;
                        ++run_size;
                    }
                } else {
                    // Find a non-ascending run
                    while (next != last) {
                        if (comp(proj(*current), proj(*next))) break;
                        ++current;
                        ++next;
                        ++run_size;
                    }
                }

                if (run_size > minrun_limit) {
                    if (begin_unsorted != last) {
                        fallback(make_sized_iterator(begin_unsorted, size_unsorted),
                                 make_sized_iterator(begin_rng, size_unsorted),
                                 compare, projection);
                        runs.push_back({ begin_rng, size_unsorted });
                        detail::reverse(begin_rng, next);
                        runs.push_back({ next, run_size });
                        begin_unsorted = last;
                        size_unsorted = 0;
                    } else {
                        detail::reverse(begin_rng, next);
                        runs.push_back({ next, run_size });
                    }

                    // Find and start a new run
                    if (next == last) break;
                    ++current;
                    ++next;
                    if (next == last) {
                        begin_unsorted = current;
                        size_unsorted = 0;
                        break;
                    }
                    if (comp(proj(*next), proj(*current))) {
                        goto find_descending;
                    } else {
                        goto find_ascending;
                    }
                } else {
                    size_unsorted += (run_size - 1);
                    if (begin_unsorted == last) {
                        begin_unsorted = begin_rng;
                    }
                }

                if (next == last) break;
            }
        }

        if (begin_unsorted != last) {
            // When run_size is added to size_unsorted, we retrieve one from
            // it because it assumes that the last element is part of the
            // next run, so we add one back here to compensate
            ++size_unsorted;
            if (size_unsorted > 1) {
                fallback(make_sized_iterator(begin_unsorted, size_unsorted),
                         make_sized_iterator(last, size_unsorted),
                         compare, projection);
            }
            runs.push_back({ last, size_unsorted });
        }

        // Last step: merge the runs
        verge::merge_runs(first, runs, std::move(compare), std::move(projection));
    }

    template<
        bool Stable,
        typename RandomAccessIterator,
        typename Compare,
        typename Projection,
        typename Fallback
    >
    auto sort(std::random_access_iterator_tag,
              RandomAccessIterator first, RandomAccessIterator last,
              difference_type_t<RandomAccessIterator> size,
              Compare compare, Projection projection, Fallback fallback)
        -> void
    {
        if (size < 128) {
            // Vergesort is inefficient for small collections
            fallback(std::move(first), std::move(last),
                     std::move(compare), std::move(projection));
            return;
        }

        // See the bidirectional overload for the description of
        // the following variables
        const difference_type_t<RandomAccessIterator> minrun_limit = size / log2(size);
        std::list<verge::run<RandomAccessIterator>> runs;
        auto begin_unsorted = last;

        // Pair of iterators to iterate through the collection
        auto current = first;
        auto next = std::next(first);

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (true) {

            // The random-access version of vergesort does not have to perform a linear
            // scan over the whole collection like the bidirectional version does:
            // instead it "jumps" n / log n elements at the same time, and start scanning
            // to the left and to the right of the position to check whether it landed in
            // a big enough run. In a collection that is truly shuffled, checking that
            // there is no such partition in the scanning area should be really fast, and
            // vergesort will perform log n jumps through the collection before falling
            // back to the adapted sorting algorithm, making it a cheap preprocessing
            // step.

            // Beginning of the current sequence
            auto begin_range = current;

            // If the last part of the collection to sort is not
            // big enough, consider that it is an unsorted sequence
            if (last - next <= minrun_limit) {
                if (begin_unsorted == last) {
                    begin_unsorted = begin_range;
                }
                break;
            }

            // Set backward iterators
            current += minrun_limit;
            next += minrun_limit;

            // Set forward iterators
            auto current2 = current;
            auto next2 = next;

            if (comp(proj(*next), proj(*current))) {
                // Found a descending run, scan to the left and to the right
                // until the limits of the run are reached
                if (Stable) {
                    // Find a strictly descending run to avoid breaking
                    // the stability of the algorithm with reverse()
                    do {
                        --current;
                        --next;
                        if (not comp(proj(*next), proj(*current))) break;
                    } while (current != begin_range);
                    if (not comp(proj(*next), proj(*current))) {
                        ++current;
                    }

                    ++current2;
                    ++next2;
                    while (next2 != last) {
                        if (not comp(proj(*next2), proj(*current2))) break;
                        ++current2;
                        ++next2;
                    }
                } else {
                    // Find a non-ascending sequence
                    do {
                        --current;
                        --next;
                        if (comp(proj(*current), proj(*next))) break;
                    } while (current != begin_range);
                    if (comp(proj(*current), proj(*next))) {
                        ++current;
                    }

                    ++current2;
                    ++next2;
                    while (next2 != last) {
                        if (comp(proj(*current2), proj(*next2))) break;
                        ++current2;
                        ++next2;
                    }
                }

                // Check whether we found a big enough sorted sequence
                if (next2 - current >= minrun_limit) {
                    detail::reverse(current, next2);
                    if ((current - begin_range) && begin_unsorted == last) {
                        begin_unsorted = begin_range;
                    }
                    if (begin_unsorted != last) {
                        fallback(begin_unsorted, current, compare, projection);
                        runs.push_back({ current, current - begin_unsorted });
                        begin_unsorted = last;
                    }
                    runs.push_back({ next2, next2 - current });
                } else {
                    // Remember the beginning of the unsorted sequence
                    if (begin_unsorted == last) {
                        begin_unsorted = begin_range;
                    }
                }
            } else {
                // Found an non-descending run, scan to the left and to
                // the right until the limits of the run are reached
                do {
                    --current;
                    --next;
                    if (comp(proj(*next), proj(*current))) break;
                } while (current != begin_range);
                if (comp(proj(*next), proj(*current))) ++current;

                ++current2;
                ++next2;
                while (next2 != last) {
                    if (comp(proj(*next2), proj(*current2))) break;
                    ++current2;
                    ++next2;
                }

                // Check whether we found a big enough sorted sequence
                if (next2 - current >= minrun_limit) {
                    if ((current - begin_range) && begin_unsorted == last) {
                        begin_unsorted = begin_range;
                    }
                    if (begin_unsorted != last) {
                        fallback(begin_unsorted, current, compare, projection);
                        runs.push_back({ current, current - begin_unsorted });
                        begin_unsorted = last;
                    }
                    runs.push_back({ next2, next2 - current });
                } else {
                    // Remember the beginning of the unsorted sequence
                    if (begin_unsorted == last) {
                        begin_unsorted = begin_range;
                    }
                }
            }

            if (next2 == last) break;

            current = std::next(current2);
            next = std::next(next2);
        }

        if (begin_unsorted != last) {
            // If there are unsorted elements left, sort them
            fallback(begin_unsorted, last, compare, projection);
            runs.push_back({ last, last - begin_unsorted });
        }

        // Last step: merge the runs
        verge::merge_runs(first, runs, std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Vergesort main interface

    template<typename Sorter>
    auto get_maybe_stable(std::true_type, Sorter&& sorter)
        -> cppsort::stable_t<Sorter>
    {
        return cppsort::stable_t<Sorter>(std::move(sorter));
    }

    template<typename Sorter>
    auto get_maybe_stable(std::false_type, Sorter&& sorter)
        -> Sorter
    {
        return std::move(sorter);
    }

    template<
        bool Stable,
        typename BidirectionalIterator,
        typename Compare,
        typename Projection,
        typename Fallback
    >
    auto sort(BidirectionalIterator first, BidirectionalIterator last,
              difference_type_t<BidirectionalIterator> size,
              Compare compare, Projection projection, Fallback fallback)
        -> void
    {
        // Adapt the fallback sorter depending on whether a stable
        // or an unstable sort is wanted
        verge::sort<Stable>(iterator_category_t<BidirectionalIterator>{},
                            std::move(first), std::move(last), size,
                            std::move(compare), std::move(projection),
                            get_maybe_stable(std::integral_constant<bool, Stable>{}, std::move(fallback)));
    }

    constexpr auto default_sorter_for_impl(std::bidirectional_iterator_tag)
        -> cppsort::quick_merge_sorter
    {
        return {};
    }

    constexpr auto default_sorter_for_impl(std::random_access_iterator_tag)
        -> cppsort::pdq_sorter
    {
        return {};
    }

    template<typename Iterator>
    constexpr auto default_sorter_for(Iterator)
        -> decltype(auto)
    {
        iterator_category_t<Iterator> category;
        return default_sorter_for_impl(category);
    }

    template<
        bool Stable,
        typename BidirectionalIterator,
        typename Compare,
        typename Projection
    >
    auto sort(BidirectionalIterator first, BidirectionalIterator last,
              difference_type_t<BidirectionalIterator> size,
              Compare compare, Projection projection)
        -> void
    {
        // Pick a default sorter based on the iterator category when
        // none is provided
        verge::sort<Stable>(first, last, size,
                            std::move(compare), std::move(projection),
                            default_sorter_for(first));
    }
}}}

#endif // CPPSORT_DETAIL_VERGESORT_H_
