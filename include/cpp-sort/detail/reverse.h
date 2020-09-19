/*
 * Copyright (c) 2016 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_REVERSE_H_
#define CPPSORT_DETAIL_REVERSE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator>
    auto reverse_impl(BidirectionalIterator first, BidirectionalIterator last,
                      std::bidirectional_iterator_tag)
        -> void
    {
        using utility::iter_swap;

        while (first != last)
        {
            if (first == --last) break;
            iter_swap(first, last);
            ++first;
        }
    }

    template<typename RandomAccessIterator>
    auto reverse_impl(RandomAccessIterator first, RandomAccessIterator last,
                      std::random_access_iterator_tag)
        -> void
    {
        if (first != last)
        {
            for (; first < --last ; ++first)
            {
                using utility::iter_swap;
                iter_swap(first, last);
            }
        }
    }

    template<typename BidirectionalIterator>
    auto reverse(BidirectionalIterator first, BidirectionalIterator last)
        -> void
    {
        reverse_impl(std::move(first), std::move(last),
                     iterator_category_t<BidirectionalIterator>{});
    }
}}

#endif // CPPSORT_DETAIL_REVERSE_H_
