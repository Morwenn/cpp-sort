/*
 * Copyright (c) 2015-2024 Morwenn
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
    // copy_cvref_t: apply cv and ref qualifiers of a given type
    // to another type
    //
    // See https://wg21.link/P1450

    template<bool RV, typename T>
    using apply_ref = mstd::conditional_t<RV, T&&, T&>;

    template<bool C, typename T>
    using apply_const = mstd::conditional_t<C, const T, T>;

    template<bool C, bool RV, typename T>
    using apply_const_ref = apply_ref<RV, apply_const<C, T>>;

    template<typename T, typename U>
    using copy_cvref_t = apply_const_ref<
        std::is_const_v<std::remove_reference_t<T>>,
        not std::is_lvalue_reference_v<T>,
        U
    >;

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


    ////////////////////////////////////////////////////////////
    // index_of: return the index of the first occurence of a
    // type in a list of type, or -1 if the type is not in the
    // list

    template<typename Needle, typename... Haystack>
    struct index_of_impl;

    template<typename Needle>
    struct index_of_impl<Needle>
    {
        static constexpr int value = -1;
    };

    template<typename Needle, typename... Tail>
    struct index_of_impl<Needle, Needle, Tail...>
    {
        static constexpr int value = 0;
    };

    template<typename Needle, typename Head, typename... Tail>
    struct index_of_impl<Needle, Head, Tail...>
    {
        static constexpr int value = index_of_impl<Needle, Tail...>::value + 1;
    };

    template<typename Needle, typename... Haystack>
    inline constexpr int index_of = index_of_impl<Needle, Haystack...>::value;
}

#endif // CPPSORT_DETAIL_TYPE_TRAITS_H_
