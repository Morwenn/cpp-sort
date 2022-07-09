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
#include <type_traits>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Alternative to std::conditional_t from C++11

    template<bool>
    struct conditional
    {
        template<typename T, typename U>
        using type = T;
    };

    template<>
    struct conditional<false>
    {
        template<typename T, typename U>
        using type = U;
    };

    template<bool B, typename T, typename U>
    using conditional_t = typename conditional<B>::template type<T, U>;

    ////////////////////////////////////////////////////////////
    // Alternative to std::enable_if_t from C++11

    template<bool>
    struct enable_if_impl {};

    template<>
    struct enable_if_impl<true>
    {
        template<typename T>
        using type = T;
    };

    template<bool B, typename T=void>
    using enable_if_t = typename enable_if_impl<B>::template type<T>;

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
    constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    template<template<typename...> typename Op, typename... Args>
    using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

    ////////////////////////////////////////////////////////////
    // Type traits to take __int128 into account even when the
    // standard library isn't instrumented but the type is still
    // available (e.g. -std=c++17)

#ifdef __SIZEOF_INT128__
    template<typename T>
    struct is_integral:
        std::is_integral<T>
    {};

    template<>
    struct is_integral<__int128_t>:
        std::true_type
    {};

    template<>
    struct is_integral<__uint128_t>:
        std::true_type
    {};

    template<typename T>
    struct is_signed:
        std::is_signed<T>
    {};

    template<>
    struct is_signed<__int128_t>:
        std::true_type
    {};

    template<typename T>
    struct is_unsigned:
        std::is_unsigned<T>
    {};

    template<>
    struct is_unsigned<__uint128_t>:
        std::true_type
    {};
#else
    template<typename T>
    using is_integral = std::is_integral<T>;

    template<typename T>
    using is_signed = std::is_signed<T>;

    template<typename T>
    using is_unsigned = std::is_unsigned<T>;
#endif

    ////////////////////////////////////////////////////////////
    // Arithmetic concepts

    template<typename T>
    concept integral = is_integral<T>::value;

    template <typename T>
    concept signed_integral = integral<T> && is_signed<T>::value;

    template <typename T>
    concept unsigned_integral = integral<T> && is_unsigned<T>::value;

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
    constexpr bool is_specialization_of_v = is_specialization_of<T, Template>::value;

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
    constexpr bool is_in_pack = is_in_pack_impl<Values...>(Value);

    template<std::size_t Value>
    constexpr bool is_in_pack<Value> = false;
}}

#endif // CPPSORT_DETAIL_TYPE_TRAITS_H_
