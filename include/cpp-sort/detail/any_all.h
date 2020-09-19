/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_ANY_ALL_H_
#define CPPSORT_DETAIL_ANY_ALL_H_

namespace cppsort
{
namespace detail
{
    constexpr auto any(bool head)
        -> bool
    {
        return head;
    }

    template<typename... Bools>
    constexpr auto any(bool head, Bools... tail)
        -> bool
    {
        return head || any(tail...);
    }

    constexpr auto all(bool head)
        -> bool
    {
        return head;
    }

    template<typename... Bools>
    constexpr auto all(bool head, Bools... tail)
        -> bool
    {
        return head && all(tail...);
    }
}}

#endif // CPPSORT_DETAIL_ANY_ALL_H_
