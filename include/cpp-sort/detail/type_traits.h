/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_TYPE_TRAITS_H_
#define CPPSORT_DETAIL_TYPE_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <tuple>
#include <type_traits>
#include <cpp-sort/mstd/type_traits.h>

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // std::nonesuch from Library Fundamentals TS v2

    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;
        void operator=(nonesuch const&) = delete;
    };

    ////////////////////////////////////////////////////////////
    // Detection idiom from Library Fundamentals TS v2

    template<
        typename Default,
        typename AlwaysVoid,
        template<typename...> typename Op, typename... Args
    >
    struct detector
    {
        using value_t = std::false_type;
        using type = Default;
    };

    template<
        typename Default,
        template<typename...> typename Op,
        typename... Args
    >
    struct detector<Default, std::void_t<Op<Args...>>, Op, Args...>
    {
        using value_t = std::true_type;
        using type = Op<Args...>;
    };

    template<template<typename...> typename Op, typename... Args>
    using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

    template<template<typename...> typename Op, typename... Args >
    inline constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    template<template<typename...> typename Op, typename... Args>
    using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

    ////////////////////////////////////////////////////////////
    // is_specialization_of: check that a given type is a
    // specialization of a given class template, with the caveat
    // that the class template can only have type template
    // parameters
    //
    // See https://wg21.link/P2098R0

    template<typename T, template<typename...> typename Template>
    struct is_specialization_of:
        std::false_type
    {};

    template<template<typename...> typename Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template>:
        std::true_type
    {};

    template<typename T, template<typename...> typename Template>
    inline constexpr bool is_specialization_of_v
        = is_specialization_of<T, Template>::value;

    ////////////////////////////////////////////////////////////
    // pack_element: retrieve the nth elements of a parameter
    // pack

    template<std::size_t N, typename... Types>
    using pack_element = std::tuple_element_t<N, std::tuple<Types...>>;

    ////////////////////////////////////////////////////////////
    // is_in_pack: check whether a given std::size_t value
    // appears in a std::size_t... parameter pack

    template<std::size_t... Values>
    constexpr auto is_in_pack_impl(std::size_t value) noexcept
        -> bool
    {
        std::size_t arr[] = { Values... };
        for (std::size_t val: arr) {
            if (val == value) {
                return true;
            }
        }
        return false;
    }

    template<std::size_t Value, std::size_t... Values>
    inline constexpr bool is_in_pack = is_in_pack_impl<Values...>(Value);

    template<std::size_t Value>
    inline constexpr bool is_in_pack<Value> = false;
}

#endif // CPPSORT_DETAIL_TYPE_TRAITS_H_
