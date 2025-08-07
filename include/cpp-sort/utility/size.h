/*
 * Copyright (c) 2015-2025 Morwenn
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

namespace cppsort::utility
{
    namespace detail
    {
        template<typename Range>
        using has_size_method_t = decltype(std::declval<const Range&>().size());
    }

    template<
        typename Range,
        typename = cppsort::detail::enable_if_t<
            cppsort::detail::is_detected_v<detail::has_size_method_t, Range>
        >
    >
    constexpr auto size(const Range& range)
        -> decltype(range.size())
    {
        return range.size();
    }

    template<
        typename Range,
        typename = cppsort::detail::enable_if_t<
            not cppsort::detail::is_detected_v<detail::has_size_method_t, Range>
        >
    >
    constexpr auto size(const Range& range)
        -> decltype(std::distance(std::begin(range), std::end(range)))
    {
        return std::distance(std::begin(range), std::end(range));
    }

    template<
        typename Range,
        typename = cppsort::detail::enable_if_t<
            not cppsort::detail::is_detected_v<detail::has_size_method_t, Range>
        >
    >
    constexpr auto size(Range& range)
        -> decltype(std::distance(std::begin(range), std::end(range)))
    {
        return std::distance(std::begin(range), std::end(range));
    }

    template<typename T, std::size_t N>
    constexpr auto size(const T (&)[N]) noexcept
        -> std::size_t
    {
        return N;
    }
}

#endif // CPPSORT_UTILITY_SIZE_H_
