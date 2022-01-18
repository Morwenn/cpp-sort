/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_RAW_CHECKERS_H_
#define CPPSORT_DETAIL_RAW_CHECKERS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include "any_all.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    // Raw checkers: type traits that check whether a specific type member
    // exists within a type. Inheriting from the checker will give to the
    // child type the same type member aliasing the same type.

    ////////////////////////////////////////////////////////////
    // iterator_category

    template<typename T, typename=void>
    struct has_iterator_category:
        std::false_type
    {};

    template<typename T>
    struct has_iterator_category<T, void_t<typename T::iterator_category>>:
        std::true_type
    {};

    template<bool, typename...>
    struct raw_check_iterator_category_impl {};

    template<typename... Sorters>
    struct raw_check_iterator_category_impl<true, Sorters...>
    {
        using iterator_category = std::common_type_t<
            typename Sorters::iterator_category...
        >;
    };

    template<typename... Sorters>
    struct raw_check_iterator_category:
        raw_check_iterator_category_impl<
            all(has_iterator_category<Sorters>::value...),
            Sorters...
        >
    {};

    ////////////////////////////////////////////////////////////
    // is_always_stable

    template<typename T, typename=void>
    struct has_is_always_stable:
        std::false_type
    {};

    template<typename T>
    struct has_is_always_stable<T, void_t<typename T::is_always_stable>>:
        std::true_type
    {};

    template<bool, typename...>
    struct raw_check_is_always_stable_impl {};

    template<typename... Sorters>
    struct raw_check_is_always_stable_impl<true, Sorters...>
    {
        using is_always_stable = std::integral_constant<
            bool,
            all(typename Sorters::is_always_stable{}()...)
        >;
    };

    template<typename... Sorters>
    struct raw_check_is_always_stable:
        raw_check_is_always_stable_impl<
            all(has_is_always_stable<Sorters>::value...),
            Sorters...
        >
    {};

    ////////////////////////////////////////////////////////////
    // is_transparent

    template<typename T, typename=void>
    struct has_is_transparent:
        std::false_type
    {};

    template<typename T>
    struct has_is_transparent<T, void_t<typename T::is_transparent>>:
        std::true_type
    {};

    template<typename T>
    constexpr bool has_is_transparent_v = has_is_transparent<remove_cvref_t<T>>::value;

    template<bool>
    struct raw_check_is_transparent_impl {};

    template<>
    struct raw_check_is_transparent_impl<true>
    {
        using is_transparent = void;
    };

    template<typename... TT>
    struct raw_check_is_transparent:
        raw_check_is_transparent_impl<
            all(has_is_transparent<TT>::value...)
        >
    {};
}}

#endif // CPPSORT_DETAIL_RAW_CHECKERS_H_
