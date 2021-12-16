/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_SIZE_H_
#define CPPSORT_UTILITY_SIZE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename Iterable>
        using has_size_method_t = decltype(std::declval<const Iterable&>().size());
    }

    template<
        typename Iterable,
        typename = cppsort::detail::enable_if_t<
            cppsort::detail::is_detected_v<detail::has_size_method_t, Iterable>
        >
    >
    constexpr auto size(const Iterable& iterable)
        -> decltype(iterable.size())
    {
        return iterable.size();
    }

    template<
        typename Iterable,
        typename = cppsort::detail::enable_if_t<
            not cppsort::detail::is_detected_v<detail::has_size_method_t, Iterable>
        >
    >
    constexpr auto size(const Iterable& iterable)
        -> decltype(std::distance(std::begin(iterable), std::end(iterable)))
    {
        return std::distance(std::begin(iterable), std::end(iterable));
    }

    template<
        typename Iterable,
        typename = cppsort::detail::enable_if_t<
            not cppsort::detail::is_detected_v<detail::has_size_method_t, Iterable>
        >
    >
    constexpr auto size(Iterable& iterable)
        -> decltype(std::distance(std::begin(iterable), std::end(iterable)))
    {
        return std::distance(std::begin(iterable), std::end(iterable));
    }

    template<typename T, std::size_t N>
    constexpr auto size(const T (&)[N]) noexcept
        -> std::size_t
    {
        return N;
    }
}}

#endif // CPPSORT_UTILITY_SIZE_H_
