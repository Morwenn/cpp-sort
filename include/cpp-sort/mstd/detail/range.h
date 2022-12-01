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

#ifndef CPPSORT_MSTD_DETAIL_RANGE_H_
#define CPPSORT_MSTD_DETAIL_RANGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <ranges>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include "begin.h"
#include "data.h"
#include "end.h"

namespace cppsort::mstd
{
    ////////////////////////////////////////////////////////////
    // range

    template<typename T>
    concept range = requires(T& value) {
        mstd::begin(value);
        mstd::end(value);
    };

    ////////////////////////////////////////////////////////////
    // input_range

    template<typename R>
    concept input_range =
        range<R> &&
        input_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // forward_range

    template<typename R>
    concept forward_range =
        input_range<R> &&
        forward_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // bidirectional_range

    template<typename R>
    concept bidirectional_range =
        forward_range<R> &&
        bidirectional_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // random_access_range

    template<typename R>
    concept random_access_range =
        bidirectional_range<R> &&
        random_access_iterator<iterator_t<R>>;

    ////////////////////////////////////////////////////////////
    // contiguous_range

    template<typename R>
    concept contiguous_range =
        random_access_range<R> &&
        contiguous_iterator<iterator_t<R>> &&
        requires(R& rng) {
            { mstd::data(rng) }
                -> std::same_as<std::add_pointer_t<
                    decltype(*std::declval<iterator_t<R>&>())
                >>;
        };

    ////////////////////////////////////////////////////////////
    // borrowed_range

    template<typename R>
    concept borrowed_range =
        range<R> && (
            std::is_lvalue_reference_v<R> ||
            std::ranges::enable_borrowed_range<std::remove_cvref_t<R>>
        );
}

#endif // CPPSORT_MSTD_DETAIL_RANGE_H_
