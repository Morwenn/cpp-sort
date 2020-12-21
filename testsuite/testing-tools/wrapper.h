/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_WRAPPER_H_
#define CPPSORT_TESTSUITE_WRAPPER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>

////////////////////////////////////////////////////////////
// Wrapper around a simple type
//
// This wrapper with a public value is mostly used to test
// pointer to data members used a projections throughout
// the test suite

template<typename T>
struct generic_wrapper
{
    T value;

    generic_wrapper() = default;
    generic_wrapper(const generic_wrapper&) = default;
    generic_wrapper(generic_wrapper&&) = default;

    constexpr generic_wrapper(const T& other_value):
        value(other_value)
    {}

    constexpr generic_wrapper(T&& other_value):
        value(std::move(other_value))
    {}

    generic_wrapper& operator=(const generic_wrapper&) = default;
    generic_wrapper& operator=(generic_wrapper&&) = default;

    constexpr auto operator=(const T& other_value)
        -> generic_wrapper&
    {
        value = other_value;
        return *this;
    }

    constexpr auto operator=(T&& other_value)
        -> generic_wrapper&
    {
        value = std::move(other_value);
        return *this;
    }
};

////////////////////////////////////////////////////////////
// Wrapper with an "order"
//
// This wrapper has an additional integral field meant to
// attach more information to it than the value it wraps,
// information which can be used to try to detect issues in
// stable sorting algorithms

template<typename T>
struct generic_stable_wrapper
{
    T value;
    int order;

    generic_stable_wrapper() = default;
    generic_stable_wrapper(const generic_stable_wrapper&) = default;
    generic_stable_wrapper(generic_stable_wrapper&&) = default;

    constexpr generic_stable_wrapper(const T& other_value):
        value(other_value)
    {}

    constexpr generic_stable_wrapper(T&& other_value):
        value(std::move(other_value))
    {}

    generic_stable_wrapper& operator=(const generic_stable_wrapper&) = default;
    generic_stable_wrapper& operator=(generic_stable_wrapper&&) = default;

    constexpr auto operator=(const T& other_value)
        -> generic_stable_wrapper&
    {
        value = other_value;
        return *this;
    }

    constexpr auto operator=(T&& other_value)
        -> generic_stable_wrapper&
    {
        value = std::move(other_value);
        return *this;
    }

    friend constexpr auto operator==(const generic_stable_wrapper& lhs, const generic_stable_wrapper& rhs)
        -> bool
    {
        return lhs.value == rhs.value
            && lhs.order == rhs.order;
    }

    friend constexpr auto operator<(const generic_stable_wrapper& lhs, const generic_stable_wrapper& rhs)
        -> bool
    {
        if (lhs.value < rhs.value) {
            return true;
        }
        if (rhs.value < lhs.value) {
            return false;
        }
        return lhs.order < rhs.order;
    }
};


#endif // CPPSORT_TESTSUITE_WRAPPER_H_
