/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_ROTATE_H_
#define CPPSORT_DETAIL_ROTATE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <numeric>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "move.h"
#include "swap_ranges.h"

namespace cppsort::detail
{
    template<typename ForwardIterator>
    auto rotate_left(ForwardIterator first, ForwardIterator last)
        -> ForwardIterator
    {
        auto tmp = mstd::iter_move(first);
        auto lm1 = detail::move(mstd::next(first), last, first);
        *lm1 = std::move(tmp);
        return lm1;
    }

    template<typename BidirectionalIterator>
    auto rotate_right(BidirectionalIterator first, BidirectionalIterator last)
        -> BidirectionalIterator
    {
        auto lm1 = mstd::prev(last);
        auto tmp = mstd::iter_move(lm1);
        auto fp1 = detail::move_backward(first, lm1, last);
        *first = std::move(tmp);
        return fp1;
    }

    template<typename ForwardIterator>
    auto rotate_forward(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
        -> ForwardIterator
    {
        auto i = middle;
        while (true) {
            mstd::iter_swap(first, i);
            ++first;
            if (++i == last) {
                break;
            }
            if (first == middle) {
                middle = i;
            }
        }
        auto r = first;
        if (first != middle) {
            i = middle;
            while (true) {
                mstd::iter_swap(first, i);
                ++first;
                if (++i == last) {
                    if (first == middle) {
                        break;
                    }
                    i = middle;
                } else if (first == middle) {
                    middle = i;
                }
            }
        }
        return r;
    }

    template<typename RandomAccessIterator>
    auto rotate_gcd(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last)
        -> RandomAccessIterator
    {
        auto m1 = middle - first;
        auto m2 = last - middle;
        if (m1 == m2) {
            detail::swap_ranges_inner(first, middle, middle);
            return middle;
        }
        auto g = std::gcd(m1, m2);
        for (auto p = first + g; p != first;) {
            auto t = mstd::iter_move(--p);
            auto p1 = p;
            auto p2 = p1 + m1;
            do {
                *p1 = mstd::iter_move(p2);
                p1 = p2;
                auto d = last - p2;
                if (m1 < d) {
                    p2 += m1;
                } else {
                    p2 = first + (m1 - d);
                }
            } while (p2 != p);
            *p1 = std::move(t);
        }
        return first + m2;
    }

    template<mstd::forward_iterator Iterator>
    auto rotate_impl(Iterator first, Iterator middle, Iterator last)
        -> Iterator
    {
        using value_type = std::iter_value_t<Iterator>;
        if constexpr (std::is_trivially_move_assignable_v<value_type>) {
            if (mstd::next(first) == middle) {
                return rotate_left(first, last);
            }
        }
        return rotate_forward(first, middle, last);
    }

    template<mstd::bidirectional_iterator Iterator>
    auto rotate_impl(Iterator first, Iterator middle, Iterator last)
        -> Iterator
    {
        using value_type = std::iter_value_t<Iterator>;
        if constexpr (std::is_trivially_move_assignable_v<value_type>) {
            if (mstd::next(first) == middle) {
                return rotate_left(first, last);
            }
            if (mstd::next(middle) == last) {
                return rotate_right(first, last);
            }
        }
        return rotate_forward(first, middle, last);
    }

    template<mstd::random_access_iterator Iterator>
    auto rotate_impl(Iterator first, Iterator middle, Iterator last)
        -> Iterator
    {
        using value_type = std::iter_value_t<Iterator>;
        if constexpr (std::is_trivially_move_assignable_v<value_type>) {
            if (mstd::next(first) == middle) {
                return rotate_left(first, last);
            }
            if (mstd::next(middle) == last) {
                return rotate_right(first, last);
            }
            return rotate_gcd(first, middle, last);
        }
        return rotate_forward(first, middle, last);
    }

    template<typename ForwardIterator>
    auto rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
        -> ForwardIterator
    {
        if (first == middle) {
            return last;
        }
        if (middle == last) {
            return first;
        }
        return rotate_impl(first, middle, last);
    }
}

#endif // CPPSORT_DETAIL_ROTATE_H_
