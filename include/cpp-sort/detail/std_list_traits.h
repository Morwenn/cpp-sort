/*
 * Copyright (c) 2016 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_STD_LIST_TRAITS_H_
#define CPPSORT_DETAIL_STD_LIST_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <list>
#include <type_traits>

namespace cppsort
{
namespace detail
{
    // Small helpers to help disambiguate the standard list
    // types in some specific scenarios

    template<typename T>
    struct is_std_list:
        std::false_type
    {};

    template<typename... Args>
    struct is_std_list<std::list<Args...>>:
        std::true_type
    {};

    template<typename T>
    struct is_std_forward_list:
        std::false_type
    {};

    template<typename... Args>
    struct is_std_forward_list<std::forward_list<Args...>>:
        std::true_type
    {};
}}

#endif // CPPSORT_DETAIL_STD_LIST_TRAITS_H_
