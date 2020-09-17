/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
#ifndef CPPSORT_UTILITY_AS_FUNCTION_H_
#define CPPSORT_UTILITY_AS_FUNCTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/static_const.h>
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        struct as_function_fn
        {
            template<typename T>
            constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::mem_fn(t)))
                -> std::enable_if_t<
                    std::is_member_pointer<cppsort::detail::remove_cvref_t<T>>::value,
                    decltype(std::mem_fn(t))
                >
            {
                return std::mem_fn(t);
            }

            template<typename T>
            constexpr auto operator()(T && t) const
                noexcept(std::is_nothrow_constructible<T, T>::value)
                -> std::enable_if_t<
                    not std::is_member_pointer<cppsort::detail::remove_cvref_t<T>>::value,
                    T
                >
            {
                return std::forward<T>(t);
            }
        };
    }

    namespace
    {
        constexpr auto&& as_function = static_const<
            detail::as_function_fn
        >::value;
    }
}}

#endif // CPPSORT_UTILITY_AS_FUNCTION_H_
