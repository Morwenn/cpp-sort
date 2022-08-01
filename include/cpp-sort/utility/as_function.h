/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
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
                -> cppsort::detail::enable_if_t<
                    std::is_member_pointer_v<std::remove_cvref_t<T>>,
                    decltype(std::mem_fn(t))
                >
            {
                return std::mem_fn(t);
            }

            template<typename T>
            constexpr auto operator()(T&& t) const
                noexcept(std::is_nothrow_constructible_v<T, T>)
                -> cppsort::detail::enable_if_t<
                    not std::is_member_pointer_v<std::remove_cvref_t<T>>,
                    T
                >
            {
                return std::forward<T>(t);
            }
        };
    }

    inline constexpr detail::as_function_fn as_function{};
}}

#endif // CPPSORT_UTILITY_AS_FUNCTION_H_
