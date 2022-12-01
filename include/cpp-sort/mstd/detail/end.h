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

#ifndef CPPSORT_MSTD_DETAIL_END_H_
#define CPPSORT_MSTD_DETAIL_END_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include "common.h"
#include "begin.h"

// Copy of _LIBCPP_AUTO_CAST, preserved as a macro to
// simplify future copy-paste-steal maintenance
#define CPPSORT_AUTO_CAST(expr) static_cast<std::decay_t<decltype((expr))>>(expr)

namespace cppsort::mstd
{
    namespace detail_end
    {
        template<typename T>
        concept member_end =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                typename iterator_t<T>;
                { CPPSORT_AUTO_CAST(rng.end()) } -> sentinel_for<iterator_t<T>>;
            };

        void end() = delete;

        template<typename T>
        concept unqualified_end =
            not member_end<T> &&
            detail::can_borrow<T> &&
            detail::class_or_union_or_enum<std::remove_cvref_t<T>> &&
            requires(T&& rng) {
                typename iterator_t<T>;
                { CPPSORT_AUTO_CAST(end(rng)) } -> sentinel_for<iterator_t<T>>;
            };

        struct fn
        {
            template<typename T, std::size_t N>
            [[nodiscard]]
            constexpr auto operator()(T (&arr)[N]) const noexcept
                requires (sizeof(T) >= 0)  // Disallow incomplete element types.
            {
                return arr + N;
            }

            template<member_end T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(rng.end())))
            {
                return CPPSORT_AUTO_CAST(rng.end());
            }

            template<unqualified_end T>
            [[nodiscard]]
            constexpr auto operator()(T&& rng) const
                noexcept(noexcept(CPPSORT_AUTO_CAST(end(rng))))
            {
                return CPPSORT_AUTO_CAST(end(rng));
            }

            void operator()(auto&&) const = delete;
        };
    }

    inline namespace cpo
    {
        inline constexpr auto end = detail_end::fn{};
    }
}

#undef CPPSORT_AUTO_CAST

#endif // CPPSORT_MSTD_DETAIL_END_H_
