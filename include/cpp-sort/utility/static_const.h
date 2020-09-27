/*
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_UTILITY_STATIC_CONST_H_
#define CPPSORT_UTILITY_STATIC_CONST_H_

namespace cppsort
{
namespace utility
{
    template<typename T>
    struct static_const
    {
        static constexpr T value{};
    };

    template<typename T>
    constexpr T static_const<T>::value;
}}

#endif // CPPSORT_UTILITY_STATIC_CONST_H_
