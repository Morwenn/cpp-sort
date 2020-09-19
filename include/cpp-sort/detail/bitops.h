/*
 * Copyright (c) 2015-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_BITOPS_H_
#define CPPSORT_DETAIL_BITOPS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <limits>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    // Returns 2^floor(log2(n)), assumes n > 0
    template<typename Unsigned>
    constexpr auto hyperfloor(Unsigned n)
        -> Unsigned
    {
        constexpr auto bound = std::numeric_limits<Unsigned>::digits / 2;
        for (std::size_t i = 1 ; i <= bound ; i <<= 1) {
            n |= (n >> i);
        }
        return n & ~(n >> 1);
    }

    // Returns floor(log2(n)), assumes n > 0

#if defined(__GNUC__) || defined(__clang__)
    constexpr auto log2(unsigned int n)
        -> unsigned int
    {
        return std::numeric_limits<unsigned int>::digits - __builtin_clz(n) - 1u;
    }

    constexpr auto log2(unsigned long n)
        -> unsigned long
    {
        return std::numeric_limits<unsigned long>::digits - __builtin_clzl(n) - 1ul;
    }

    constexpr auto log2(unsigned long long n)
        -> unsigned long long
    {
        return std::numeric_limits<unsigned long long>::digits - __builtin_clzll(n) - 1ull;
    }
#endif

    template<typename Integer>
    constexpr auto log2(Integer n)
        -> Integer
    {
        Integer log = 0;
        while (n >>= 1) {
            ++log;
        }
        return log;
    }

    // Halves a positive number, using unsigned division if possible

    template<typename Integer>
    constexpr auto half(Integer value)
        -> std::enable_if_t<std::is_integral<Integer>::value, Integer>
    {
        return static_cast<Integer>(static_cast<std::make_unsigned_t<Integer>>(value) / 2);
    }

    template<typename T>
    constexpr auto half(T value)
        -> std::enable_if_t<not std::is_integral<T>::value, T>
    {
        return value / 2;
    }
}}

#endif // CPPSORT_DETAIL_BITOPS_H_
