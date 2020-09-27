/*
 * Copyright (c) 2016-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_CHECKERS_H_
#define CPPSORT_DETAIL_CHECKERS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include "any_all.h"
#include "raw_checkers.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // High-level checkers (check with sorter_traits)

    template<bool, typename...>
    struct check_iterator_category_impl {};

    template<typename... Sorters>
    struct check_iterator_category_impl<true, Sorters...>
    {
        using iterator_category = std::common_type_t<
            typename sorter_traits<Sorters>::iterator_category...
        >;
    };

    template<typename... Sorters>
    struct check_iterator_category:
        check_iterator_category_impl<
            all(has_iterator_category<sorter_traits<Sorters>>::value...),
            Sorters...
        >
    {};

    template<bool, typename...>
    struct check_is_always_stable_impl {};

    template<typename... Sorters>
    struct check_is_always_stable_impl<true, Sorters...>
    {
        using is_always_stable = std::integral_constant<
            bool,
            all(typename sorter_traits<Sorters>::is_always_stable{}()...)
        >;
    };

    template<typename... Sorters>
    struct check_is_always_stable:
        check_is_always_stable_impl<
            all(has_is_always_stable<sorter_traits<Sorters>>::value...),
            Sorters...
        >
    {};
}}

#endif // CPPSORT_DETAIL_CHECKERS_H_
