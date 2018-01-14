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
#ifndef CPPSORT_DETAIL_TIMSORT_UTILS_H_
#define CPPSORT_DETAIL_TIMSORT_UTILS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "compiler.h"
#include "memcpy_algos.h"
#include "minrun.h"
#include "../iterator_traits.h"
#include "../move.h"
#include "../upper_bound.h"

namespace cppsort
{
namespace detail
{
    /*
     * Number of consecutive elements for which galloping would be a win over
     * either linear or binary search.
     */
    constexpr std::size_t gallop_win_dist = 7;

    /*
     * @brief 	 Semantically equivalent to std::upper_bound(), except requires
     *        	 random access iterators.
     * @param begin     Iterator to the first element in the range to search.
     * @param end       Past-the-end iterator to the range to search.
     * @param value     The value to search for.
     * @param compare   The comparison function to use.
     *
     * Similar to std::upper_bound, but first finds the k such that
     * comp(value, begin[2**(k - 1) - 1]) and not comp(begin[2**k - 1], value)
     * and then returns the equivalent of
     *  	std::upper_bound(begin + 2**(k - 1), begin + 2**k - 1, value, comp)
     *
     */
    template<typename Iterator, typename T, typename Compare>
    auto gallop_upper_bound(Iterator begin, Iterator end, const T& value, Compare compare)
        -> Iterator
    {
        auto&& comp = utility::as_function(compare);

        std::size_t i = 0;
        std::size_t len = end - begin;
        for (; i < len && not comp(value, begin[i]) ; i = 2 * i + 1); // loop

        if (len > i) {
            len = i;
        }
        len -= (i / 2);
        begin += (i / 2);
        // hand-rolled std::upper_bound.
        while (len > 0) {
            i = len / 2;
            if (comp(value, begin[i])) {
                len = i;
            } else {
                begin += (i + 1);
                len -= (i + 1);
            }
        }
        return begin;
    }

    /*
     * @brief        Performs a leftwards rotation by one.
     * @param begin  Random access iterator to the first element in the range
     *  	  	 to be rotated.
     * @param end    Past-the-end random access iterator to the range to be
     *		 rotated.
     *
     * Semantically equivalent to std::rotate(begin, begin + 1, end) except
     * random access iterators are required.
     */
    template<typename Iterator>
    auto tim_rotate_left(Iterator begin, Iterator end)
        -> void
    {
        using utility::iter_move;
        using utility::iter_swap;

        // heuristic for deciding whether to use a temporary + range-move
        // if a type is smaller than a 'pointer-size-capacity' type like std::vector or std::string,
        // then implement the rotate as:
        //	 move-to-temporary -> std::move_backward -> move-from-temporary
        // otherwise implement as a series of swaps.
        //
        // this heuristic is, of course, evaluated at compile-time
        //
        // benchmarking across a number of different cases shows that this usually wins over
        // a call to std::rotate()
        using value_type = value_type_t<Iterator>;
        constexpr std::size_t use_temporary_upper_limit = 3 * sizeof(void*);
        constexpr bool use_temporary = sizeof(value_type) < use_temporary_upper_limit;
        if (use_temporary)
        {
            // for small types, implement using a temporary.
            if (end - begin > 1) {
                auto temp = iter_move(std::prev(end));
                detail::move_backward(begin, std::prev(end), end);
                *begin = std::move(temp);
            }
        }
        else
        {
            // for large types, implement as a series of adjacent swaps
            for (end -= (end > begin) ; end > begin ; --end) {
                iter_swap(end - 1, end);
            }
        }
    }

    /*
     * @brief        Insertion sort the range [begin, end), where [begin, mid) is
     * 		 already sorted.
     * @param begin     Random access iterator to the first element in the range.
     * @param begin     Random access iterator to the first out-of-order element
     * 		 in the range.
     * @param end       Past-the-end random access iterator to the range.
     * @param compare   Comparator to use.
     */
    template<typename Iterator, typename Compare>
    auto finish_insertion_sort(Iterator begin, Iterator mid, Iterator end,
                               Compare compare)
        -> void
    {
        using utility::iter_swap;
        auto&& comp = utility::as_function(compare);
        using value_type = value_type_t<Iterator>;

        if (std::is_scalar<value_type>::value && (
            std::is_same<Compare, std::less<>>::value ||
            std::is_same<Compare, std::less<value_type>>::value ||
            std::is_same<Compare, std::greater<>>::value ||
            std::is_same<Compare, std::greater<value_type>>::value
        ))
        {
            // if types are cheap to compare and cheap to copy, do a linear search
            // instead of a binary search
            while (mid < end) {
                for (auto pos = mid; pos > begin && comp(*pos, pos[-1]); --pos) {
                    iter_swap(pos - 1, pos);
                }
                ++mid;
            }
        }
        else
        {
            // do the canonical 'swap it with the one before it' insertion sort up
            // to max_minrun<value_type>() / 4 elements
            for (const auto stop = begin + std::min(max_minrun<value_type>() / 4, std::size_t(end - begin)); mid < stop; ++mid) {
                for (auto pos = mid; pos > begin && comp(*pos, pos[-1]); --pos) {
                    iter_swap(pos - 1, pos);
                }
            }
            // finish it off with a binary insertion sort
            for (; mid < end ; ++mid) {
                tim_rotate_left(detail::upper_bound(begin, mid, *mid, compare), mid + 1);
            }
        }

    }
}}

#endif // CPPSORT_DETAIL_TIMSORT_UTILS_H_
