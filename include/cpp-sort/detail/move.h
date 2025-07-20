/*
 * Copyright (c) 2019-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MOVE_H_
#define CPPSORT_DETAIL_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "memory.h"
#include "type_traits.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // move

    template<typename InputIterator, typename OutputIterator>
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        if constexpr (cppsort::detail::has_iter_move_v<InputIterator>) {
            for (; first != last; ++first, (void) ++result) {
                using utility::iter_move;
                *result = iter_move(first);
            }
            return result;
        } else {
            return std::move(first, last, result);
        }
    }

    ////////////////////////////////////////////////////////////
    // move_backward

    template<typename InputIterator, typename OutputIterator>
    auto move_backward(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        if constexpr (cppsort::detail::has_iter_move_v<InputIterator>) {
            while (first != last) {
                using utility::iter_move;
                *--result = iter_move(--last);
            }
            return result;
        } else {
            return std::move_backward(first, last, result);
        }
    }

    ////////////////////////////////////////////////////////////
    // uninitialized_move

    template<typename InputIterator, typename T>
    auto uninitialized_move(InputIterator first, InputIterator last, T* result, destruct_n<T>& destroyer)
        -> T*
    {
        constexpr bool is_trivial_enough =
            std::is_trivial_v<value_type_t<InputIterator>> &&
            std::is_trivial_v<T>;

        if (is_trivial_enough) {
            return detail::move(first, last, result);
        } else {
            for (; first != last; ++first, (void) ++result, ++destroyer) {
                using utility::iter_move;
                ::new(static_cast<void*>(result)) T(iter_move(first));
            }
            return result;
        }
    }
}

#endif // CPPSORT_DETAIL_MOVE_H_
