/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_DETAIL_STABLE_PARTITION_H_
#define CPPSORT_DETAIL_STABLE_PARTITION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "memory.h"
#include "rotate.h"

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator, typename Predicate>
    auto stable_partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred,
                          difference_type_t<BidirectionalIterator> len,
                          temporary_buffer<rvalue_type_t<BidirectionalIterator>>& buffer)
        -> BidirectionalIterator
    {
        using rvalue_type = rvalue_type_t<BidirectionalIterator>;
        using utility::iter_move;
        using utility::iter_swap;

        // *first is known to be false
        // *last is known to be true
        // len >= 2
        if (len == 2) {
            iter_swap(first, last);
            return last;
        }
        if (len == 3) {
            auto m = first;
            if (pred(*++m)) {
                iter_swap(first, m);
                iter_swap(m, last);
                return last;
            }
            iter_swap(m, last);
            iter_swap(first, m);
            return m;
        }
        if (len <= buffer.size()) {
            // The buffer is big enough to use
            destruct_n<rvalue_type> d(0);
            std::unique_ptr<rvalue_type, destruct_n<rvalue_type>&> h(buffer.data(), d);
            // Move the falses into the temporary buffer, and the trues to the front of the line
            // Update first to always point to the end of the trues
            auto ptr = buffer.data();
            ::new (ptr) rvalue_type(iter_move(first));
            ++d;
            ++ptr;
            auto it = first;
            while (++it != last) {
                if (pred(*it)) {
                    *first = iter_move(it);
                    ++first;
                } else {
                    ::new (ptr) rvalue_type(iter_move(it));
                    ++d;
                    ++ptr;
                }
            }
            // move *last, known to be true
            *first = iter_move(it);
            it = ++first;
            // All trues now at start of range, all falses in buffer
            // Move falses back into range, but don't mess up first which points to first false
            for (auto t2 = buffer.data() ; t2 < ptr ; ++t2, (void) ++it)
                *it = iter_move(t2);
            // h destructs moved-from values out of the temp buffer, but doesn't deallocate buffer
            return first;
        }
        // Else not enough buffer, do in place
        // len >= 4
        auto m = first;
        auto len2 = len / 2;  // len2 >= 2
        std::advance(m, len2);
        // recurse on [first, m-1], except reduce m-1 until *(m-1) is true, *first know to be false
        // F????????????????T
        // f       m        l
        auto m1 = m;
        auto first_false = first;
        auto len_half = len2;
        while (not pred(*--m1)) {
            if (m1 == first) {
                goto first_half_done;
            }
            --len_half;
        }
        // F???TFFF?????????T
        // f   m1  m        l
        first_false = stable_partition(first, m1, pred, len_half, buffer);
    first_half_done:
        // TTTFFFFF?????????T
        // f  ff   m        l
        // recurse on [m, last], except increase m until *(m) is false, *last know to be true
        m1 = m;
        auto second_false = last;
        ++second_false;
        len_half = len - len2;
        while (pred(*m1)) {
            if (++m1 == last) {
                goto second_half_done;
            }
            --len_half;
        }
        // TTTFFFFFTTTF?????T
        // f  ff   m  m1    l
        second_false = stable_partition(m1, last, std::move(pred), len_half, buffer);
    second_half_done:
        // TTTFFFFFTTTTTFFFFF
        // f  ff   m    sf  l
        return detail::rotate(first_false, m, second_false);
        // TTTTTTTTFFFFFFFFFF
        //         |
    }

    template<typename BidirectionalIterator, typename Predicate>
    auto stable_partition(BidirectionalIterator first, BidirectionalIterator last,
                          difference_type_t<BidirectionalIterator> size,
                          Predicate predicate)
        -> BidirectionalIterator
    {
        using difference_type = difference_type_t<BidirectionalIterator>;
        auto&& pred = utility::as_function(predicate);

        // might want to make this a function of trivial assignment
        constexpr difference_type alloc_limit = 4;

        // Either prove all true and return first or point to first false
        while (true) {
            if (first == last) {
                return first;
            }
            if (not pred(*first)) {
                break;
            }
            --size;
            ++first;
        }
        // first points to first false, everything prior to first is already set.
        // Either prove [first, last) is all false and return first, or point last to last true
        do {
            --size;
            if (first == --last) {
                return first;
            }
        } while (not pred(*last));
        // We now have a reduced range [first, last]
        // *first is known to be false
        // *last is known to be true
        // len >= 2
        auto len = size + 1;
        temporary_buffer<rvalue_type_t<BidirectionalIterator>> buffer(nullptr);
        if (len >= alloc_limit) {
            buffer.try_grow(len);
        }
        return stable_partition(first, last, std::move(pred), len, buffer);
    }
}}

#endif // CPPSORT_DETAIL_STABLE_PARTITION_H_
