/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_IS_TRANSPARENT_H_
#define CPPSORT_TESTSUITE_IS_TRANSPARENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <stdexcept>
#include <type_traits>
#include <cpp-sort/detail/type_traits.h>

////////////////////////////////////////////////////////////
// Test that a function object has an is_transparent type

template<typename T, typename=void>
struct has_is_transparent:
    std::false_type
{};

template<typename T>
struct has_is_transparent<T, cppsort::detail::void_t<typename T::is_transparent>>:
    std::true_type
{};

template<typename T>
constexpr bool has_is_transparent_v = has_is_transparent<
    cppsort::detail::remove_cvref_t<T>
>::value;

////////////////////////////////////////////////////////////
// Couple of types to ensure that transparent comparators
// fulfill their promises

struct is_transparent_helper_stored;
struct is_transparent_helper_compared;

struct is_transparent_helper_stored
{
    int value = 0;

    is_transparent_helper_stored(const is_transparent_helper_stored&) = default;
    is_transparent_helper_stored(is_transparent_helper_stored&&) = default;
    is_transparent_helper_stored& operator=(const is_transparent_helper_stored&) = default;
    is_transparent_helper_stored& operator=(is_transparent_helper_stored&&) = default;

    is_transparent_helper_stored(int value):
        value(value)
    {}

    is_transparent_helper_stored(const is_transparent_helper_compared&)
    {
        throw std::runtime_error("is_transparent_helper_stored constructed from is_transparent_helper_compared");
    }

    friend auto operator<(const is_transparent_helper_stored& lhs, const is_transparent_helper_stored& rhs)
        -> bool
    {
        return lhs.value < rhs.value;
    }
};

struct is_transparent_helper_compared
{
    int value = 0;

    is_transparent_helper_compared(const is_transparent_helper_compared&) = default;
    is_transparent_helper_compared(is_transparent_helper_compared&&) = default;
    is_transparent_helper_compared& operator=(const is_transparent_helper_compared&) = default;
    is_transparent_helper_compared& operator=(is_transparent_helper_compared&&) = default;

    is_transparent_helper_compared(int value):
        value(value)
    {}

    friend auto operator<(const is_transparent_helper_compared& lhs, const is_transparent_helper_compared& rhs)
        -> bool
    {
        return lhs.value < rhs.value;
    }
};

inline auto operator<(const is_transparent_helper_stored& lhs, const is_transparent_helper_compared& rhs)
    -> bool
{
    return lhs.value < rhs.value;
}

inline auto operator<(const is_transparent_helper_compared& lhs, const is_transparent_helper_stored& rhs)
    -> bool
{
    return lhs.value < rhs.value;
}

#endif // CPPSORT_TESTSUITE_IS_TRANSPARENT_H_
