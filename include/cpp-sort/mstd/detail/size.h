/*
 * Copyright (c) 2022-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_DETAIL_SIZE_H_
#define CPPSORT_MSTD_DETAIL_SIZE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <ranges>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "../../detail/bitops.h"
#include "begin.h"
#include "common.h"
#include "end.h"

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // size

    namespace detail_size
    {
        void size() = delete;

        template<typename T>
        concept size_enabled = not std::ranges::disable_sized_range<std::remove_cvref_t<T>>;

        template<typename T>
        concept member_size =
            size_enabled<T> &&
            detail::class_or_union<T> &&
            requires(T&& value) {
                { auto(value.size()) } -> detail::integer_like;
            };

        template<typename T>
        concept unqualified_size =
            size_enabled<T> &&
            not member_size<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& value) {
                { auto(size(value)) } -> detail::integer_like;
            };

        template<typename T>
        concept difference =
            not member_size<T> &&
            not unqualified_size<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& value) {
                { mstd::begin(value) } -> forward_iterator;
                { mstd::end(value) } -> sized_sentinel_for<decltype(mstd::begin(std::declval<T>()))>;
            };

        struct fn
        {
            // `[range.prim.size]`: the array case (for rvalues).
            template<typename T, std::size_t Size>
            [[nodiscard]]
            constexpr auto operator()(T (&&)[Size]) const noexcept
                -> std::size_t
            {
                return Size;
            }

            // `[range.prim.size]`: the array case (for lvalues).
            template<typename T, std::size_t Size>
            [[nodiscard]]
            constexpr auto operator()(T (&)[Size]) const noexcept
                -> std::size_t
            {
                return Size;
            }

            // `[range.prim.size]`: `auto(t.size())` is a valid expression.
            template<member_size T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(auto(value.size())))
                 -> detail::integer_like auto
            {
                return auto(value.size());
            }

            // `[range.prim.size]`: `auto(size(t))` is a valid expression.
            template<unqualified_size T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(auto(size(value))))
                 ->  detail::integer_like auto
            {
                return auto(size(value));
            }

            // [range.prim.size]: the `to-unsigned-like` case.
            template<difference T>
            [[nodiscard]]
            constexpr auto operator()(T&& value) const
                noexcept(noexcept(cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value))))
                -> decltype(cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value)))
            {
                return cppsort::detail::as_unsigned(mstd::end(value) - mstd::begin(value));
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto size = detail_size::fn{};
    }

    ////////////////////////////////////////////////////////////
    // sized_range

    template<typename T>
    concept sized_range =
        range<T> &&
        requires(T& t) {
            mstd::size(t);
        };
}

#endif // CPPSORT_MSTD_DETAIL_SIZE_H_
