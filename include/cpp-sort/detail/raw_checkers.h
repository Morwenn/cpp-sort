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

namespace cppsort::detail
{
    // Raw checkers: type traits that check whether a specific type member
    // exists within a type. Inheriting from the checker will give to the
    // child type the same type member aliasing the same type.

    ////////////////////////////////////////////////////////////
    // iterator_category

    template<typename T>
    inline constexpr bool has_iterator_category_v = requires {
        typename T::iterator_category;
    };

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
            (has_iterator_category_v<Sorters> && ...),
            Sorters...
        >
    {};

    ////////////////////////////////////////////////////////////
    // is_always_stable

    template<typename T>
    inline constexpr bool has_is_always_stable_v = requires {
        typename T::is_always_stable;
    };

    template<bool, typename...>
    struct raw_check_is_always_stable_impl {};

    template<typename... Sorters>
    struct raw_check_is_always_stable_impl<true, Sorters...>
    {
        using is_always_stable = std::bool_constant<
            (typename Sorters::is_always_stable{}() && ...)
        >;
    };

    template<typename... Sorters>
    struct raw_check_is_always_stable:
        raw_check_is_always_stable_impl<
            (has_is_always_stable_v<Sorters> && ...),
            Sorters...
        >
    {};

    ////////////////////////////////////////////////////////////
    // is_transparent

    template<typename T>
    inline constexpr bool has_is_transparent_v = requires {
        typename T::is_transparent;
    };

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
            (has_is_transparent_v<TT> && ...)
        >
    {};
}

#endif // CPPSORT_DETAIL_RAW_CHECKERS_H_
