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

#ifndef CPPSORT_MSTD_DETAIL_COMMON_H_
#define CPPSORT_MSTD_DETAIL_COMMON_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <ranges>
#include <type_traits>
#include <utility>

namespace cppsort::mstd::detail
{
    ////////////////////////////////////////////////////////////
    // different_from

    template<typename T, typename U>
    concept different_from = not std::same_as<T, U>;

    ////////////////////////////////////////////////////////////
    // class_or_union[_or_enum]

    template<typename T>
    concept class_or_union =
        std::is_class_v<T> ||
        std::is_union_v<T>;

    template<typename T>
    concept class_or_union_or_enum =
        std::is_class_v<T> ||
        std::is_union_v<T> ||
        std::is_enum_v<T>;

    ////////////////////////////////////////////////////////////
    // can_borrow

    template<typename T>
    concept can_borrow =
        std::is_lvalue_reference_v<T> ||
        std::ranges::enable_borrowed_range<std::remove_cvref_t<T>>;

    ////////////////////////////////////////////////////////////
    // with_reference/can_reference

    template<typename T>
    using with_reference = T&;

    template<typename T>
    concept can_reference = requires {
        typename with_reference<T>;
    };

    ////////////////////////////////////////////////////////////
    // boolean_testable

    template<typename T>
    concept boolean_testable_impl =
        std::convertible_to<T, bool>;

    template<typename T>
    concept boolean_testable =
        boolean_testable_impl<T> &&
        requires(T&& value) {
            { not std::forward<T>(value) } -> boolean_testable_impl;
        };

    ////////////////////////////////////////////////////////////
    // integer_like & friends

#ifdef _MSC_VER
    template<typename T>
    concept integer_like =
        std::_Integer_like<T>;

    template<typename T>
    concept signed_integer_like =
        std::_Signed_integer_like<T>;
#else
    template<typename T>
    concept integer_like =
        integral<T> &&
        not std::is_same_v<std::remove_cv_t<T>, bool>;

    template<typename T>
    concept signed_integer_like =
        signed_integral<T> &&
        not std::is_same_v<std::remove_cv_t<T>, bool>;
#endif

    template<typename T>
    concept unsigned_integer_like =
        integer_like<T> &&
        not signed_integer_like<T>;
}

#endif // CPPSORT_MSTD_DETAIL_COMMON_H_
