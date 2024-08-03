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

#ifndef CPPSORT_MSTD_DETAIL_DATA_H_
#define CPPSORT_MSTD_DETAIL_DATA_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <memory>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include "common.h"
#include "begin.h"

namespace cppsort::mstd
{
    namespace detail_data
    {
        template<typename T>
        concept ptr_to_object =
            std::is_pointer_v<T> &&
            std::is_object_v<std::remove_pointer_t<T>>;

        template<typename T>
        concept member_data =
            detail::can_borrow<T> &&
            detail::class_or_union<std::remove_cvref_t<T>> &&
            requires(T&& t) {
                { auto(t.data()) } -> ptr_to_object;
            };

        template<typename T>
        concept mstd_begin_invocable =
            not member_data<T> &&
            detail::can_borrow<T> &&
            requires(T&& t) {
                { mstd::begin(t) } -> contiguous_iterator;
            };

        struct fn
        {
            template<member_data T>
            constexpr auto operator()(T&& t) const
                noexcept(noexcept(t.data()))
            {
                return t.data();
            }

            template<mstd_begin_invocable T>
            constexpr auto operator()(T&& t) const
                noexcept(noexcept(std::to_address(mstd::begin(t))))
            {
                return std::to_address(mstd::begin(t));
            }
        };
    }

    inline namespace cpo
    {
        inline constexpr auto data = detail_data::fn{};
    }
}

#endif // CPPSORT_MSTD_DETAIL_DATA_H_
