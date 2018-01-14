/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Timothy Van Slyke
 * Modified in 2018 by Morwenn for inclusion into cpp-sort
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_TIMSORT_TIMSORT_H_
#define CPPSORT_DETAIL_TIMSORT_TIMSORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <limits>
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "compiler.h"
#include "minrun.h"
#include "timsort_stack_buffer.h"
#include "utils.h"
#include "../iterator_traits.h"
#include "../lower_bound.h"
#include "../memory.h"
#include "../reverse.h"
#include "../upper_bound.h"

namespace cppsort
{
namespace detail
{
    template<typename Iterator, typename Compare>
    struct TimSort
    {
        using rvalue_reference = std::decay_t<rvalue_reference_t<Iterator>>;

        /*
         * @brief Perform a timsort on the range [begin_it, end_it).
         * @param begin_it   Random access iterator to the first element in the range.
         * @param end_it     Past-the-end random access iterator.
         * @param compare    Comparator to use.
         */
        using value_type = value_type_t<Iterator>;
        TimSort(Iterator begin_it, Iterator end_it, Compare compare):
            stack_buffer{},
            start(begin_it),
            stop(end_it),
            position(begin_it),
            compare(compare),
            minrun(compute_minrun<value_type>(end_it - begin_it)),
            min_gallop(default_min_gallop)
        {
            // try_get_cached_heap_buffer(heap_buffer);
            fill_run_stack();
            collapse_run_stack();
            // try_cache_heap_buffer(heap_buffer);
        }

        /*
         * Continually push runs onto the run stack, merging adjacent runs
         * to resolve invariants on the size of the runs in the stack along
         * the way.
         */
        auto fill_run_stack()
            -> void
        {
            // push the first two runs on to the run stack, unless there's
            // only one run.
            push_next_run();
            if (not (position < stop)) return;
            push_next_run();
            while (position < stop) {
                // resolve invariants before pushing the next run
                resolve_invariants();
                push_next_run();
            }
        }

        /*
         * Grand finale. Keep merging the top 2 runs on the stack until there
         * is only one left.
         */
        auto collapse_run_stack()
            -> void
        {
            for (auto count = stack_buffer.run_count() - 1 ; count > 0 ; --count) {
                merge_BC();
            }
        }

        /*
         * Get the next run of already-sorted elements.  If the length of the
         * natural run is less than minrun, force it to size with an insertion sort.
         */

        auto push_next_run()
            -> void
        {
            auto&& comp = utility::as_function(compare);

            // check if the next run is at least two elements long
            const std::size_t remain = stop - position;
            if (COMPILER_LIKELY_(remain > 1)) {
                std::size_t idx = 2;

                // descending?
                if (comp(position[1], position[0])) {
                    // see how long it is descending for and then reverse it
                    while (idx < remain and comp(position[idx], position[idx - 1])) {
                        ++idx;
                    }
                    detail::reverse(position, position + idx);
                }

                // ascending
                // even if the run was initially descending, after reversing it the
                // following elements may form an ascending continuation of the
                // now-reversed run.
                // unconditionally attempt to continue the ascending run
                while (idx < remain and not comp(position[idx], position[idx - 1])) {
                    ++idx;
                }
                // if needed, force the run to 'minrun' elements, or until all elements
                // in the range are exhausted (whichever comes first) with an insertion
                // sort.
                if (idx < remain and idx < minrun) {
                    auto extend_to = std::min(minrun, remain);
                    finish_insertion_sort(position, position + idx, position + extend_to, compare);
                    idx = extend_to;
                }
                // advance 'position' by the length of the run we just found
                position += idx;
            } else {
                // only one element
                position = stop;
            }
            // push the run on to the run stack.
            stack_buffer.push(position - start);
        }

        /*
         * MERGE PATTERN STUFF
         */

        /*
         * Assume the run stack has the following form:
         * 	[ ..., W, X, Y, Z]
             * Where Z is the length of the run at the top of the run stack.
         *
         * This function continually merges with Y with Z or X with Y until
         * the following invariants are satisfied:
         * 	(1) X > Y + Z
         * 	  (1.1) W > X + Y
         *      (2) Y > Z
         *
         * If (1) or (1.1) are not satisfied, Y is merged with the smaller of
         * X and Z. Otherwise if (2) is not satisfied, Y and Z are merged.
         *
         * This gives a reasonable upper bound on the size of the run stack.
         *
         *   NOTE:
         *   invariant (1.1) implements a fix for a bug in the original
         *   implementation described here:
         *   http://envisage-project.eu/wp-content/uploads/2015/02/sorting.pdf
         *
         *   The original description of these invariants written by Tim Peters
         *   accounts for only the top three runs and refers to them as: A, B,
         *   and C. This implementation uses Tim's labelling scheme in some
         *   function names, but implements the corrected invariants as
         *   described above.
         *
         * ALSO NOTE:
         *
         * For more details see:
         * https://github.com/python/cpython/blob/master/Objects/listsort.txt
         */
        auto resolve_invariants()
            -> void
        {
            // Check all of the invariants in a loop while there are at least
            // two runs.
            auto run_count = stack_buffer.run_count();
            do {
                if ((run_count > 2 && stack_buffer.merge_ABC_case_1()) ||
                    (run_count > 3 && stack_buffer.merge_ABC_case_2())) {
                    if (stack_buffer.merge_AB()) {
                        merge_AB();
                    } else {
                        merge_BC();
                    }
                } else if (stack_buffer.merge_BC()) {
                    merge_BC();
                } else {
                    break;
                }
                --run_count;
            } while(run_count > 1);
        }

        /* RUN STACK STUFF */

        /*
         * @brief Merge the second-to-last run with the last run.
         */
        auto merge_BC()
            -> void
        {
            merge_runs(start + get_offset<2>(),
                       start + get_offset<1>(),
                       start + get_offset<0>());
            stack_buffer.template remove_run<1>();
        }

        /*
         * @brief Merge the third-to-last run with the second-to-last run.
         */
        auto merge_AB()
            -> void
        {
            merge_runs(start + get_offset<3>(),
                       start + get_offset<2>(),
                       start + get_offset<1>());
            stack_buffer.template remove_run<2>();
        }

        /*
         * @brief Fetches the offset at the Ith position down from the top of
         * the run stack.
         *
         * Each offset on the run stack is the end position of each run.  So
         * if we wanted two iterators defining the run at the top of the stack
         * we would do something like this:
         *
         * 	auto begin = this->start + this->get_offset<1>();
         * 	auto end   = this->start + this->get_offset<0>();
         */
        template<std::size_t I>
        auto get_offset() const noexcept
        {
            return stack_buffer.template get_offset<I>();
        }

        /*
         * MERGE/SORT IMPL STUFF
         */

        /*
         * @brief Merges the range [begin, mid) with the range [mid, end).
         * @param begin  Iterator to the first item in the left range.
         * @param mid    Iterator to the last/first item in the left/right range.
         * @param end    Iterator to the last item in the right range.
         *
         * Requires:
         *     begin < mid and mid < end.
         *     std::is_sorted(begin, mid, this->compare)
         *     std::is_sorted(mid, end, this->compare)
         */
        auto merge_runs(Iterator begin, Iterator mid, Iterator end)
            -> void
        {
            auto&& comp = utility::as_function(compare);

            // We're going to need to copy the smaller of these ranges
            // into a temporary buffer (which may end up having to be on
            // the heap). Before we do any copying, try to reduce the
            // effective size of each range by looking for position 'p1'
            // in [begin, mid) that mid[0] belongs in. Similarly look
            // for mid[-1] in [mid, end) and call that 'p2'.
            // We then only need to merge [p1, mid) with [mid, p2).
            //
            // This also may reduce the number of items we need to copy
            // into the temporary buffer, and if we're lucky, it may even
            // make it possible to use just the space we have on the stack.

            begin = gallop_upper_bound(begin, mid, *mid, compare);
            end = gallop_upper_bound(
                std::make_reverse_iterator(end),
                std::make_reverse_iterator(mid),
                mid[-1],
                [&comp](auto&& a, auto&& b) mutable {
                    return comp(std::forward<decltype(b)>(b), std::forward<decltype(a)>(a));
                }
            ).base();

            if (COMPILER_LIKELY_(begin < mid or mid < end)) {
                if ((end - mid) > (mid - begin)) {
                    // merge from the left
                    do_merge(begin, mid, end, compare);
                } else {
                    // merge from the right
                    do_merge(
                        std::make_reverse_iterator(end),
                        std::make_reverse_iterator(mid),
                        std::make_reverse_iterator(begin),
                        [&comp](auto&& a, auto&& b) mutable {
                           // reverse the comparator
                            return comp(std::forward<decltype(b)>(b),
                                        std::forward<decltype(a)>(a));
                        }
                    );
                }
            }
        }

        /*
         * @brief Merges the range [begin, mid) with the range [mid, end).
         * @param begin  Iterator to the first item in the left range.
         * @param mid    Iterator to the last/first item in the left/right range.
         * @param end    Iterator to the last item in the right range.
         * @param cmp    Comparator to merge with respect to.
         *
         * Requires:
         *     begin < mid and mid < end.
         *     std::is_sorted(begin, mid, cmp)
         *     std::is_sorted(mid, end, cmp)
         */
        template<typename Iter1, typename Iter2, typename Cmp>
        auto do_merge_inner(Iter1 begin, Iter1 mid, Iter1 end,
                            Iter2 buff_begin, Iter2 buff_end, Cmp cmp)
            -> std::enable_if_t<
                can_forward_memcpy_v<Iter1>
            >
        {
            std::memcpy(buff_begin, get_memcpy_iterator(begin),
                        (mid - begin) * sizeof(value_type));
            gallop_merge(buff_begin, buff_end, mid, end, begin, cmp);
        }

        template<typename Iter1, typename Iter2, typename Cmp>
        auto do_merge_inner(Iter1 begin, Iter1 mid, Iter1 end,
                            Iter2 buff_begin, Iter2 buff_end, Cmp cmp)
            -> std::enable_if_t<
                can_reverse_memcpy_v<Iter1>
            >
        {
            std::memcpy(buff_begin, get_memcpy_iterator(mid - 1),
                        (mid - begin) * sizeof(value_type));
            gallop_merge(std::make_reverse_iterator(buff_end),
                         std::make_reverse_iterator(buff_begin),
                         mid, end, begin, cmp);
        }

        template<typename Iter1, typename Iter2, typename Cmp>
        auto do_merge_inner(Iter1 begin, Iter1 mid, Iter1 end,
                            Iter2 buff_begin, Iter2 buff_end, Cmp cmp)
            -> std::enable_if_t<
                not(can_forward_memcpy_v<Iter1> || can_reverse_memcpy_v<Iter1>)
            >
        {
            detail::move(begin, mid, buff_begin);
            gallop_merge(buff_begin, buff_end, mid, end, begin, cmp);
        }

        template<typename Iter, typename Cmp>
        auto do_merge(Iter begin, Iter mid, Iter end, Cmp cmp)
            -> void
        {
            // check to see if we can use the run stack as a temporary buffer
            if (stack_buffer.can_acquire_merge_buffer(begin, mid)) {
                // allocate the merge buffer on the stack
                auto stack_mem = stack_buffer.move_to_merge_buffer(begin, mid);
                gallop_merge(stack_mem, stack_mem + (mid - begin),
                             mid, end, begin, cmp);
            } else {
                // TODO: clean this up
                // fall back to heap memory for the merge buffer
                // try to use memcpy if possible

                auto buffer_size = std::distance(begin, mid);
                std::unique_ptr<rvalue_reference, operator_deleter> buffer(
                    static_cast<rvalue_reference*>(::operator new(buffer_size * sizeof(rvalue_reference)))
                );
                destruct_n<rvalue_reference> d(0);
                std::unique_ptr<rvalue_reference, destruct_n<rvalue_reference>&> h2(buffer.get(), d);

                do_merge_inner(begin, mid, end, buffer.get(), buffer.get() + buffer_size, cmp);
            }
        }

        /*
         * @brief Implementation of the merge routine.
         * @param lbegin  Iterator to the begining of the left range.
         * @param lend    Iterator to the end of the left range.
         * @param rbegin  Iterator to the begining of the right range.
         * @param rend    Iterator to the end of the right range.
         * @param cmp     Comparator.  Either this->comp or a lambda reversing
         *	  	  the direction of this->comp.
         *
         * This merge rountine has been fine-tuned to take advantage of the
         * the preconditions imposed by the other components of this timsort
         * implementation. This is not a general-purpose merge routine and
         * it should not be used outside of this implementation.
         *
         * Most of the logic of this merge routine is implemented inline, and
         * questionable constructs (such as gotos) are used where benchmarking
         * has shown that they speed up the routine. The code isn't pretty,
         * but it's damn fast.
         *
         * requires:
         * 	[lbegin, lend) does not overlap with [rbegin, rend)
         *	lend - lbegin > 0 and rend - rbegin > 0
         *      cmp(*rbegin, *lbegin)
         *      cmp(rend[-1], lend[-1])
         */
        template<typename LeftIt, typename RightIt, typename DestIt, typename Cmp>
        auto gallop_merge(LeftIt lbegin, LeftIt lend, RightIt rbegin, RightIt rend,
                          DestIt dest, Cmp cmp)
            -> void
        {
            auto&& comp = utility::as_function(cmp);

            // God bless you if you're reading this. I'll try to explain
            // what I'm doing here to the best of my ability.  Much like the
            // merge routine in CPython's list_sort(), there are two modes
            // of operation, linear and galloping. Linear mode is simply
            // the classic merge routine with the caveat that after one of
            // the two ranges wins 'this->min_gallop' times in a row, we
            // jump into the gallop loop.
            //
            // To understand the gallop loop, see the required reading here:
            // https://github.com/python/cpython/blob/master/Objects/listsort.txt
            //
            // Here are few refinements we make over CPython's list_sort().
            // Each of these were benchmarked on a variety of test cases
            // to ensure that they actually sped up the merge routine.
            // 	- When switching to galloping mode from linear mode,
            // 	  jump straight to the left range if we were already
            // 	  merging the left range in linear mode, and jump
            // 	  do the opposite if we were merging the right range.
            //      - If we're already in galloping mode and we're changing
            //        to the other range (e.g. just finished the left range
            // 	  and are now going to gallop in the right range) set
            // 	  start galloping at index 1 instead of 0. This saves
            // 	  a redundant comparison.
            // 	  This is achieved by setting num_galloped to 1 just
            //	  before the entry points for the gotos in the linear
            // 	  mode loop. Every time we enter the linear mode loop,
            //    num_galloped is set to zero again.
            // 	- Since, before each call to gallop_merge() we first
            // 	  make sure to cut off all elements at the end of the
            //	  right range that are already in the correct place, we
            // 	  know that we will exhaust the right range before the
            // 	  left. This means we can skip any "lbegin < lend"
            // 	  checks and instead just unconditionally copy the
            // 	  remainder of the left range into the destination
            // 	  range once we've used up the right range.
            // 	- Another small optimization that speeds up the merge
            // 	  if we're sorting trivially copyable types, is using
            // 	  memcpy() when copying from the left range. Since
            // 	  the left range is always the one that got copied into
            // 	  the merge buffer, we know that the left range doesn't
            //	  overlap with the destination range.  Some additional
            // 	  compile-time checks are done to ensure that memcpy()
            // 	  can be done without invoking UB (e.g. checking that
            // 	  'dest' is a contiguous iterator or that both 'DestIt'
            // 	  and 'LeftIt' are reverse contiguous iterators and
            // 	  ensuring that values being sorted are trivially
            // 	  copyable).
            for (std::size_t num_galloped=0, lcount=0, rcount=0 ;;) {
                // LINEAR SEARCH MODE
                // do a naive merge until evidence shows that galloping may be faster.

                // set lcount to 1 if num_galloped > 0. if we got here from exiting the
                // gallop loop, then we already copied one extra element from the
                // left range and we should count that towards lcount here.
                for (lcount=(num_galloped > 0), rcount=0, num_galloped=0;;) {
                    if (comp(*rbegin, *lbegin)) {
                        // move from the right-hand-side
                        *dest = std::move(*rbegin);
                        ++dest;
                        ++rbegin;
                        ++rcount;
                        // merge is done.  copy the remaining elements from the left range and return
                        if (not (rbegin < rend)) {
                            move_or_memcpy(lbegin, lend, dest);
                            return;
                        } else if (rcount >= min_gallop) {
                            goto gallop_right; // continue this run in galloping mode
                        }
                        lcount = 0;
                    } else {
                        // move from the left-hand side
                        *dest = std::move(*lbegin);
                        ++dest;
                        ++lbegin;
                        ++lcount;
                        // don't need to check if we reached the end.  that will happen on the right-hand-side
                        if (lcount >= min_gallop) {
                            goto gallop_left; // continue this run in galloping mode
                        }
                        rcount = 0;
                    }
                }
                COMPILER_UNREACHABLE_;
                // GALLOP SEARCH MODE
                while (lcount >= gallop_win_dist or rcount >= gallop_win_dist) {
                    // decrement min_gallop every time we continue the gallop loop
                    if (min_gallop > 1) {
                        --min_gallop;
                    }

                    // we already know the result of the first comparison, so set num_galloped to 1 and skip it
                    num_galloped = 1;
                    gallop_left: // when jumping here from the linear merge loop, num_galloped is set to zero
                    lcount = lend - lbegin;
                    // gallop through the left range
                    while ((num_galloped < lcount) && not comp(*rbegin, lbegin[num_galloped])) {
                        num_galloped = 2 * num_galloped + 1;
                    }
                    if (lcount > num_galloped) {
                        lcount = num_galloped;
                    }
                    // do a binary search in the narrowed-down region
                    lcount = detail::upper_bound(lbegin + (num_galloped / 2), lbegin + lcount, *rbegin, cmp) - lbegin;
                    dest = move_or_memcpy(lbegin, lbegin + lcount, dest);
                    lbegin += lcount;

                    // don't need to check if we reached the end.  that will happen on the right-hand-side
                    // we already know the result of the first comparison, so set num_galloped to 1 and skip it
                    num_galloped = 1;
                    gallop_right: // when jumping here from the linear merge loop, num_galloped is set to zero
                    rcount = rend - rbegin;
                    // gallop through the right range
                    while ((num_galloped < rcount) && comp(rbegin[num_galloped], *lbegin)) {
                        num_galloped = 2 * num_galloped + 1;
                    }
                    if (rcount > num_galloped) {
                        rcount = num_galloped;
                    }
                    // do a binary search in the narrowed-down region
                    rcount = detail::lower_bound(rbegin + (num_galloped / 2), rbegin + rcount, *lbegin, cmp) - rbegin;
                    dest = std::move(rbegin, rbegin + rcount, dest);
                    rbegin += rcount;

                    // merge is done.  copy the remaining elements from the left range and return
                    if (not (rbegin < rend)) {
                        move_or_memcpy(lbegin, lend, dest);
                        return;
                    }
                }
                // exiting the loop means we just finished galloping
                // through the right-hand side.  We know for a fact
                // that 'not cmp(*rbegin, *lbegin)', so do one copy for free.
                ++min_gallop;
                *dest = std::move(*lbegin);
                ++dest;
                ++lbegin;
            }
            COMPILER_UNREACHABLE_;
        }

        /*
         * Stack-allocated stack data structure that holds location (end position)
         * of each run. Bottom of the stack always holds 0.
         * Empty stack space is used for merge buffer when possible.
         */
        timsort_stack_buffer<std::size_t, rvalue_reference> stack_buffer;
        /* 'begin' iterator to the range being sorted. */
        const Iterator start;
        /* 'end' iterator to the range being sorted. */
        const Iterator stop;
        /*
         * Iterator to keep track of how far we've scanned into the range to be
         * sorted. [start, position) contains already-found runs while
         * [position, stop) is still untouched. When position == end, the run stack
         * is collapsed.
         */
        Iterator position;
        /* Comparator used to sort the range. */
        Compare compare;
        /* Minimum length of a run */
        const std::size_t minrun;
        /*
         * Minimum number of consecutive elements for which one side of the
         * merge must "win" in a row before switching from galloping mode to
         * linear mode.
         */
        std::size_t min_gallop = default_min_gallop;

        static constexpr std::size_t default_min_gallop = gallop_win_dist;
    };

    template<typename Iterator, typename Compare>
    auto timsort(Iterator begin, Iterator end, Compare compare)
        -> void
    {
        using value_type = value_type_t<Iterator>;
        std::size_t len = std::distance(begin, end);
        if (len > max_minrun<value_type>()) {
            TimSort<Iterator, Compare>(begin, end, std::move(compare));
        } else {
            finish_insertion_sort(begin, begin + (end > begin), end, std::move(compare));
        }
    }
}}

#include "undef_compiler.h"

#endif // CPPSORT_DETAIL_TIMSORT_TIMSORT_H_
