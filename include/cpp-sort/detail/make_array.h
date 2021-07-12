/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MAKE_ARRAY_H_
#define CPPSORT_DETAIL_MAKE_ARRAY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <utility>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // The constexpr capabilities of std::array are seriously
    // underpowered in C++14, so we instead fill C arrays in
    // some algorithms and use the following function to turn
    // them into std::array

    template<typename T, std::size_t N, std::size_t... Indices>
    constexpr auto make_array_impl(T(&arr)[N], std::index_sequence<Indices...>)
        -> std::array<T, N>
    {
        return std::array<T, N>{{ arr[Indices]... }};
    }

    template<typename T, std::size_t N>
    constexpr auto make_array(T(&arr)[N])
        -> std::array<T, N>
    {
        using indices = std::make_index_sequence<N>;
        return make_array_impl(arr, indices{});
    }
}}

#endif // CPPSORT_DETAIL_MAKE_ARRAY_H_
