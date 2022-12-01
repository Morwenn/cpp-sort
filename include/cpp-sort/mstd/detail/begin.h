/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef CPPSORT_MSTD_DETAIL_BEGIN_H_
#define CPPSORT_MSTD_DETAIL_BEGIN_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include "common.h"

// Copy of _LIBCPP_AUTO_CAST, preserved as a macro to
// simplify future copy-paste-steal maintenance
#define CPPSORT_AUTO_CAST(expr) static_cast<std::decay_t<decltype((expr))>>(expr)

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // begin

    namespace detail_begin
    {
        template<typename T>
        concept member_begin =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                { CPPSORT_AUTO_CAST(rng.begin()) } -> input_or_output_iterator;
            };

        void begin() = delete;

        template<class T>
        concept unqualified_begin =
            not member_begin<T> &&
            detail::can_borrow<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                { CPPSORT_AUTO_CAST(begin(rng)) } -> input_or_output_iterator;
            };

        struct fn
        {
            template<typename T>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + 0;
            }

            template<typename T, std::size_t N>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[N]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + 0;
            }

            template<member_begin T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(rng.begin())))
            {
                return CPPSORT_AUTO_CAST(rng.begin());
            }

            template<unqualified_begin T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(begin(rng))))
            {
                return CPPSORT_AUTO_CAST(begin(rng));
            }

            void operator()(auto&&) const = delete;
        };
    }

    inline namespace cpo
    {
        inline constexpr auto begin = detail_begin::fn{};
    }

    ////////////////////////////////////////////////////////////
    // iterator_t

    template<typename T>
    using iterator_t = decltype(mstd::begin(std::declval<T&>()));
}

#undef CPPSORT_AUTO_CAST

#endif // CPPSORT_MSTD_DETAIL_BEGIN_H_
