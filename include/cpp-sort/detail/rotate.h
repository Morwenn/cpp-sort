/*
 * Copyright (c) 2016-2021 Morwenn
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
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "move.h"
#include "swap_ranges.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator>
    auto rotate_left(ForwardIterator first, ForwardIterator last)
        -> ForwardIterator
    {
        using utility::iter_move;

        auto tmp = iter_move(first);
        ForwardIterator lm1 = detail::move(std::next(first), last, first);
        *lm1 = std::move(tmp);
        return lm1;
    }

    template<typename BidirectionalIterator>
    auto rotate_right(BidirectionalIterator first, BidirectionalIterator last)
        -> BidirectionalIterator
    {
        using utility::iter_move;

        BidirectionalIterator lm1 = std::prev(last);
        auto tmp = iter_move(lm1);
        BidirectionalIterator fp1 = detail::move_backward(first, lm1, last);
        *first = std::move(tmp);
        return fp1;
    }

    template<typename ForwardIterator>
    auto rotate_forward(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
        -> ForwardIterator
    {
        using utility::iter_swap;

        ForwardIterator i = middle;
        while (true)
        {
            iter_swap(first, i);
            ++first;
            if (++i == last)
                break;
            if (first == middle)
                middle = i;
        }
        ForwardIterator r = first;
        if (first != middle)
        {
            i = middle;
            while (true)
            {
                iter_swap(first, i);
                ++first;
                if (++i == last)
                {
                    if (first == middle)
                        break;
                    i = middle;
                }
                else if (first == middle)
                    middle = i;
            }
        }
        return r;
    }

    template<typename Integral>
    auto gcd(Integral x, Integral y)
        -> Integral
    {
        do
        {
            Integral t = x % y;
            x = y;
            y = t;
        } while (y);
        return x;
    }

    template<typename RandomAccessIterator>
    auto rotate_gcd(RandomAccessIterator first, RandomAccessIterator middle,
                    RandomAccessIterator last)
        -> RandomAccessIterator
    {
        using utility::iter_move;
        using difference_type = difference_type_t<RandomAccessIterator>;

        const difference_type m1 = middle - first;
        const difference_type m2 = last - middle;
        if (m1 == m2)
        {
            detail::swap_ranges_inner(first, middle, middle);
            return middle;
        }
        const difference_type g = gcd(m1, m2);
        for (RandomAccessIterator p = first + g; p != first;)
        {
            auto t  = iter_move(--p);
            RandomAccessIterator p1 = p;
            RandomAccessIterator p2 = p1 + m1;
            do
            {
                *p1 = iter_move(p2);
                p1 = p2;
                const difference_type d = last - p2;
                if (m1 < d)
                    p2 += m1;
                else
                    p2 = first + (m1 - d);
            } while (p2 != p);
            *p1 = std::move(t);
        }
        return first + m2;
    }

    template<typename ForwardIterator>
    auto rotate_impl(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                     std::forward_iterator_tag)
        -> ForwardIterator
    {
        using value_type = value_type_t<ForwardIterator>;
        if (std::is_trivially_move_assignable<value_type>::value)
        {
            if (std::next(first) == middle)
                return rotate_left(first, last);
        }
        return rotate_forward(first, middle, last);
    }

    template<typename BidirectionalIterator>
    auto rotate_impl(BidirectionalIterator first, BidirectionalIterator middle, BidirectionalIterator last,
                     std::bidirectional_iterator_tag)
        -> BidirectionalIterator
    {
        using value_type = value_type_t<BidirectionalIterator>;
        if (std::is_trivially_move_assignable<value_type>::value)
        {
            if (std::next(first) == middle)
                return rotate_left(first, last);
            if (std::next(middle) == last)
                return rotate_right(first, last);
        }
        return rotate_forward(first, middle, last);
    }

    template<typename RandomAccessIterator>
    auto rotate_impl(RandomAccessIterator first, RandomAccessIterator middle, RandomAccessIterator last,
                     std::random_access_iterator_tag)
        -> RandomAccessIterator
    {
        using value_type = value_type_t<RandomAccessIterator>;
        if (std::is_trivially_move_assignable<value_type>::value)
        {
            if (std::next(first) == middle)
                return rotate_left(first, last);
            if (std::next(middle) == last)
                return rotate_right(first, last);
            return rotate_gcd(first, middle, last);
        }
        return rotate_forward(first, middle, last);
    }

    template<typename ForwardIterator>
    auto rotate(ForwardIterator first, ForwardIterator middle, ForwardIterator last)
        -> ForwardIterator
    {
        if (first == middle)
            return last;
        if (middle == last)
            return first;
        return rotate_impl(first, middle, last, iterator_category_t<ForwardIterator>{});
    }
}}

#endif // CPPSORT_DETAIL_ROTATE_H_
