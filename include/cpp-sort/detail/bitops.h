/*
 * Copyright (c) 2015-2021 Morwenn
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
#include "../detail/attributes.h"
#include "../detail/config.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace detail
{
    // Cast signed value to unsigned one
    template<typename Integer>
    CPPSORT_ATTRIBUTE_NODISCARD
    constexpr auto as_unsigned(Integer value)
        -> std::make_unsigned_t<Integer>
    {
        return static_cast<std::make_unsigned_t<Integer>>(value);
    }

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

    // Returns 2^ceil(log2(n)), assumes n > 0
    template<typename Unsigned>
    constexpr auto hyperceil(Unsigned n)
        -> Unsigned
    {
        constexpr auto bound = std::numeric_limits<Unsigned>::digits / 2;
        --n;
        for (std::size_t i = 1 ; i <= bound ; i <<= 1) {
            n |= (n >> i);
        }
        return ++n;
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

    // Returns ceil(log2(n)), assumes n > 0
    template<typename Integer>
    constexpr auto ceil_log2(Integer n)
        -> Integer
    {
        Integer log = 0;
        --n;
        while (n > 0) {
            ++log;
            n >>= 1;
        }
        return log;
    }

    // Halves a positive number, using unsigned division if possible

    template<typename Integer>
    constexpr auto half(Integer value)
        -> detail::enable_if_t<std::is_integral<Integer>::value, Integer>
    {
        return static_cast<Integer>(as_unsigned(value) / 2);
    }

    template<typename T>
    constexpr auto half(T value)
        -> detail::enable_if_t<not std::is_integral<T>::value, T>
    {
        return value / 2;
    }

    // Returns whether an integer has a single bit set, generally
    // used to check whether an integer is a power of 2,
    // assumes n >= 0

    template<typename Integer>
    constexpr auto has_single_bit(Integer n) noexcept
        -> bool
    {
        CPPSORT_ASSERT(n >= 0);
        auto x = as_unsigned(n);
        return x != 0 && (x & (x - 1)) == 0;
    }

    // Left bit rotation
    template<typename Unsigned>
    constexpr auto rotl(Unsigned x, int s)
        -> Unsigned
    {
        CPPSORT_ASSERT(s > 0);
        constexpr auto n = std::numeric_limits<Unsigned>::digits;
        return (x << s) | (x >> (n - s));
    }
}}

#endif // CPPSORT_DETAIL_BITOPS_H_
