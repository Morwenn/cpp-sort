/*
 * Copyright (c) 2015-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_IS_IN_PACK_H_
#define CPPSORT_DETAIL_IS_IN_PACK_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<std::size_t Value, std::size_t... Values>
    struct is_in_pack_impl;

    template<
        std::size_t Value,
        std::size_t Head,
        std::size_t... Tail
    >
    struct is_in_pack_impl<Value, Head, Tail...>:
        conditional_t<
            Value == Head,
            std::true_type,
            is_in_pack_impl<Value, Tail...>
        >
    {};

    template<std::size_t Value>
    struct is_in_pack_impl<Value>:
        std::false_type
    {};

    template<std::size_t Value, std::size_t... Values>
    constexpr bool is_in_pack = is_in_pack_impl<Value, Values...>::value;
}}

#endif // CPPSORT_DETAIL_IS_IN_PACK_H_
