// -*- C++ -*-
//===-------------------------- algorithm ---------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
// //  Modified in 2016-2017 by Morwenn for inclusion into cpp-sort
//
//===----------------------------------------------------------------------===//
#ifndef CPPSORT_DETAIL_MOVE_H_
#define CPPSORT_DETAIL_MOVE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstring>
#include <iterator>
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // unwrap_iter

    template<typename Iterator>
    auto unwrap_iter(Iterator it)
        -> Iterator
    {
        return it;
    }

    template<typename T>
    auto unwrap_iter(std::move_iterator<T*> it)
        -> std::enable_if_t<
            std::is_trivially_copy_assignable<T>::value,
            T*
        >
    {
        return it.base();
    }

    ////////////////////////////////////////////////////////////
    // move

    template<typename InputIterator, typename OutputIterator>
    auto move_impl(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        using utility::iter_move;

        for (; first != last; ++first, (void) ++result) {
            *result = iter_move(first);
        }
        return result;
    }

    template<typename T, typename U>
    auto move_impl(T* first, T* last, U* result)
        -> std::enable_if_t<
            std::is_same<std::remove_const_t<T>, U>::value &&
            std::is_trivially_copy_assignable<U>::value,
            U*
        >
    {
        const std::size_t n = static_cast<std::size_t>(last - first);
        if (n > 0) {
            std::memmove(result, first, n * sizeof(U));
        }
        return result + n;
    }

    template<typename InputIterator, typename OutputIterator>
    auto move(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        return move_impl(unwrap_iter(first), unwrap_iter(last), unwrap_iter(result));
    }

    ////////////////////////////////////////////////////////////
    // move_backward

    template<typename InputIterator, typename OutputIterator>
    auto move_backward_impl(InputIterator first, InputIterator last, OutputIterator result)
        -> OutputIterator
    {
        using utility::iter_move;

        while (first != last) {
            *--result = iter_move(--last);
        }
        return result;
    }

    template<typename T, typename U>
    auto move_backward_impl(T* first, T* last, U* result)
        -> std::enable_if_t<
            std::is_same<std::remove_const_t<T>, U>::value &&
            std::is_trivially_copy_assignable<U>::value,
            U*
        >
    {
        const std::size_t n = static_cast<std::size_t>(last - first);
        if (n > 0) {
            result -= n;
            std::memmove(result, first, n * sizeof(U));
        }
        return result;
    }

    template<typename BidirectionalIterator1, typename BidirectionalIterator2>
    auto move_backward(BidirectionalIterator1 first, BidirectionalIterator1 last,
                       BidirectionalIterator2 result)
        -> BidirectionalIterator2
    {
        return move_backward_impl(unwrap_iter(first), unwrap_iter(last), unwrap_iter(result));
    }
}}

#endif // CPPSORT_DETAIL_MOVE_H_
