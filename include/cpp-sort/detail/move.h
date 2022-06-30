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
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "memory.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // move

    template<typename InputIterator, typename OutputIterator>
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> detail::enable_if_t<
            not cppsort::detail::has_iter_move_v<InputIterator>,
            OutputIterator
        >
    {
        return std::move(first, last, result);
    }

    template<typename InputIterator, typename OutputIterator>
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> detail::enable_if_t<
            cppsort::detail::has_iter_move_v<InputIterator>,
            OutputIterator
        >
    {
        for (; first != last; ++first, (void) ++result) {
            using utility::iter_move;
            *result = iter_move(first);
        }
        return result;
    }

    ////////////////////////////////////////////////////////////
    // move_backward

    template<typename InputIterator, typename OutputIterator>
    auto move_backward(InputIterator first, InputIterator last, OutputIterator result)
        -> detail::enable_if_t<
            not cppsort::detail::has_iter_move_v<InputIterator>,
            OutputIterator
        >
    {
        return std::move_backward(first, last, result);
    }

    template<typename InputIterator, typename OutputIterator>
    auto move_backward(InputIterator first, InputIterator last, OutputIterator result)
        -> detail::enable_if_t<
            cppsort::detail::has_iter_move_v<InputIterator>,
            OutputIterator
        >
    {
        while (first != last) {
            using utility::iter_move;
            *--result = iter_move(--last);
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
            using utility::iter_move;
            ::new(static_cast<void*>(result)) T(iter_move(first));
        }
        return result;
    }

    template<typename InputIterator, typename T>
    auto uninitialized_move(InputIterator first, InputIterator last, T* result, destruct_n<T>& destroyer)
        -> T*
    {
        using truth_type = std::integral_constant<bool,
            std::is_trivial<value_type_t<InputIterator>>::value &&
            std::is_trivial<T>::value
        >;
        return uninitialized_move_impl(truth_type{}, std::move(first), std::move(last),
                                       std::move(result), destroyer);
    }
}}

#endif // CPPSORT_DETAIL_MOVE_H_
