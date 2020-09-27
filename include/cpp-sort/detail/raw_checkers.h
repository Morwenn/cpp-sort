/*
 * Copyright (c) 2016-2018 Morwenn
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
    ////////////////////////////////////////////////////////////
    // Raw checkers (check with the type itself)

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
}}

#endif // CPPSORT_DETAIL_RAW_CHECKERS_H_
