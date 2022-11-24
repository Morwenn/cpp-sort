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

#ifndef CPPSORT_DETAIL_PARTITION_H_
#define CPPSORT_DETAIL_PARTITION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/mstd/iterator.h>

namespace cppsort::detail
{
    template<mstd::forward_iterator Iterator, typename Predicate>
    constexpr auto partition(Iterator first, Iterator last, Predicate pred)
        -> Iterator
    {
        while (true) {
            if (first == last) {
                return first;
            }
            if (not pred(*first)) {
                break;
            }
            ++first;
        }
        for (auto p = first; ++p != last;) {
            if (pred(*p)) {
                mstd::iter_swap(first, p);
                ++first;
            }
        }
        return first;
    }

    template<mstd::bidirectional_iterator Iterator, typename Predicate>
    constexpr auto partition(Iterator first, Iterator last, Predicate pred)
        -> Iterator
    {
        while (true) {
            while (true) {
                if (first == last) {
                    return first;
                }
                if (not pred(*first)) {
                    break;
                }
                ++first;
            }
            do {
                if (first == --last) {
                    return first;
                }
            } while (not pred(*last));
            mstd::iter_swap(first, last);
            ++first;
        }
    }
}

#endif // CPPSORT_DETAIL_PARTITION_H_
