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
#include <iterator>
#include <utility>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename Predicate, typename ForwardIterator>
    auto partition_impl(ForwardIterator first, ForwardIterator last, Predicate pred,
                        std::forward_iterator_tag)
        -> ForwardIterator
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
                using utility::iter_swap;
                iter_swap(first, p);
                ++first;
            }
        }
        return first;
    }

    template<typename Predicate, typename BidirectionalIterator>
    auto partition_impl(BidirectionalIterator first, BidirectionalIterator last, Predicate pred,
                        std::bidirectional_iterator_tag)
        -> BidirectionalIterator
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
            using utility::iter_swap;
            iter_swap(first, last);
            ++first;
        }
    }

    template<typename ForwardIterator, typename Predicate>
    auto partition(ForwardIterator first, ForwardIterator last, Predicate pred)
        -> ForwardIterator
    {
        return partition_impl<Predicate&>(
            std::move(first), std::move(last), pred,
            iterator_category_t<ForwardIterator>{}
        );
    }
}}

#endif // CPPSORT_DETAIL_PARTITION_H_
