/*
 * Copyright (c) 2016-2022 Morwenn
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
#include <algorithm>
#include <iterator>
#include <utility>
#include <cpp-sort/mstd/iterator.h>

namespace cppsort::detail
{
    template<mstd::bidirectional_iterator Iterator>
    auto reverse(Iterator first, Iterator last)
        -> void
    {
        while (first != last) {
            if (first == --last) break;
            mstd::iter_swap(first, last);
            ++first;
        }
    }

#if defined(_USE_STD_VECTOR_ALGORITHMS) && _USE_STD_VECTOR_ALGORITHMS
    template<mstd::random_access_iterator Iterator>
        requires (not detail::has_iter_move_v<Iterator>)
    auto reverse(Iterator first, Iterator last)
        -> void
    {
        std::reverse(first, last);
    }
#endif

    template<mstd::random_access_iterator Iterator>
#if defined(_USE_STD_VECTOR_ALGORITHMS) && _USE_STD_VECTOR_ALGORITHMS
        requires detail::has_iter_move_v<Iterator>
#endif
    auto reverse_impl(Iterator first, Iterator last)
        -> void
    {
        if (first != last) {
            for (; first < --last; ++first) {
                mstd::iter_swap(first, last);
            }
        }
    }
}

#endif // CPPSORT_DETAIL_REVERSE_H_
