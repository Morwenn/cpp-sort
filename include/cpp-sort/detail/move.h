/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MOVE_H_
#define CPPSORT_DETAIL_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <memory>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include "memory.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // move

    template<typename InputIterator, typename OutputIterator>
        requires (not cppsort::detail::has_iter_move_v<InputIterator>)
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        return std::move(first, last, result);
    }

    template<typename InputIterator, typename OutputIterator>
        requires cppsort::detail::has_iter_move_v<InputIterator>
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        for (; first != last; ++first, (void) ++result) {
            *result = mstd::iter_move(first);
        }
        return result;
    }

    ////////////////////////////////////////////////////////////
    // move_backward

    template<typename InputIterator, typename OutputIterator>
        requires (not cppsort::detail::has_iter_move_v<InputIterator>)
    auto move_backward(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        return std::move_backward(first, last, result);
    }

    template<typename InputIterator, typename OutputIterator>
        requires cppsort::detail::has_iter_move_v<InputIterator>
    auto move_backward(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        while (first != last) {
            *--result = mstd::iter_move(--last);
        }
        return result;
    }

    ////////////////////////////////////////////////////////////
    // uninitialized_move

    template<typename InputIterator, typename T>
    auto uninitialized_move_impl(std::true_type, InputIterator first, InputIterator last,
                                 T* result, destruct_n<T>&)
        -> T*
    {
        return detail::move(first, last, result);
    }

    template<typename InputIterator, typename T>
    auto uninitialized_move_impl(std::false_type, InputIterator first, InputIterator last,
                                 T* result, destruct_n<T>& destroyer)
        -> T*
    {
        for (; first != last; ++first, (void) ++result, ++destroyer) {
            std::construct_at(result, mstd::iter_move(first));
        }
        return result;
    }

    template<typename InputIterator, typename T>
    auto uninitialized_move(InputIterator first, InputIterator last, T* result, destruct_n<T>& destroyer)
        -> T*
    {
        using truth_type = std::bool_constant<
            std::is_trivial_v<std::iter_value_t<InputIterator>> &&
            std::is_trivial_v<T>
        >;
        return uninitialized_move_impl(truth_type{}, std::move(first), std::move(last),
                                       std::move(result), destroyer);
    }
}

#endif // CPPSORT_DETAIL_MOVE_H_
