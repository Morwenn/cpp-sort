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
    // std::void_t from C++17

    template<typename...>
    using void_t = void;

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
        template<typename...> class Op, typename... Args
    >
    struct detector
    {
        using value_t = std::false_type;
        using type = Default;
    };

    template<
        typename Default,
        template<typename...> class Op,
        typename... Args
    >
    struct detector<Default, void_t<Op<Args...>>, Op, Args...>
    {
        using value_t = std::true_type;
        using type = Op<Args...>;
    };

    template<template<typename...> class Op, typename... Args>
    using is_detected = typename detector<nonesuch, void, Op, Args...>::value_t;

    template<template<typename...> class Op, typename... Args >
    constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    template<template<typename...> class Op, typename... Args>
    using detected_t = typename detector<nonesuch, void, Op, Args...>::type;

    ////////////////////////////////////////////////////////////
    // std::invoke_result from C++17

#ifdef __cpp_lib_is_invocable

    template<typename Func, typename... Args>
    using invoke_result = std::invoke_result<Func, Args...>;

    template<typename Func, typename... Args>
    using invoke_result_t = std::invoke_result_t<Func, Args...>;

#else

    template<typename Func, typename... Args>
    struct invoke_result:
        std::result_of<Func(Args...)>
    {};

    template<typename T, std::size_t N, typename... Args>
    struct invoke_result<T[N], Args...> {};

    template<typename Func, typename... Args>
    using invoke_result_t = typename invoke_result<Func, Args...>::type;

#endif

    ////////////////////////////////////////////////////////////
    // std::is_invocable from C++17

#ifdef __cpp_lib_is_invocable

    template<typename Func, typename... Args>
    using is_invocable = std::is_invocable<Func, Args...>;

    template<typename Func, typename... Args>
    constexpr bool is_invocable_v = std::is_invocable_v<Func, Args...>;

    template<typename Ret, typename Func, typename... Args>
    using is_invocable_r = std::is_invocable_r<Ret, Func, Args...>;

    template<typename Ret, typename Func, typename... Args>
    constexpr bool is_invocable_r_v = std::is_invocable_r_v<Ret, Func, Args...>;

#else

    template<typename Ret, typename, typename Func, typename... Args>
    struct is_invocable_impl:
        std::false_type
    {};

    template<typename Func, typename... Args>
    struct is_invocable_impl<void, void_t<invoke_result_t<Func, Args...>>, Func, Args...>:
        std::true_type
    {};

    template<typename Ret, typename Func, typename... Args>
    struct is_invocable_impl<Ret, void_t<invoke_result_t<Func, Args...>>, Func, Args...>:
        std::is_convertible<invoke_result_t<Func, Args...>, Ret>
    {};

    template<typename Ret, typename T, std::size_t N, typename... Args>
    struct is_invocable_impl<Ret, void, T[N], Args...>:
        std::false_type
    {};

    template<typename Func, typename... Args>
    struct is_invocable:
        is_invocable_impl<void, void, Func, Args...>
    {};

    template<typename Func, typename... Args>
    constexpr bool is_invocable_v = is_invocable<Func, Args...>::value;

    template<typename Ret, typename Func, typename... Args>
    struct is_invocable_r:
        is_invocable_impl<Ret, void, Func, Args...>
    {};

    template<typename Ret, typename Func, typename... Args>
    constexpr bool is_invocable_r_v = is_invocable_r<Ret, Func, Args...>::value;

#endif

    ////////////////////////////////////////////////////////////
    // std::conjunction from C++17

    template<typename...>
    struct conjunction:
        std::true_type
    {};

    template<typename Head>
    struct conjunction<Head>:
        Head
    {};

    template<typename Head, typename... Tail>
    struct conjunction<Head, Tail...>:
        conditional_t<Head::value != false, conjunction<Tail...>, Head>
    {};

    ////////////////////////////////////////////////////////////
    // std::disjunction from C++17

    template<typename...>
    struct disjunction:
        std::false_type
    {};

    template<typename Head>
    struct disjunction<Head>:
        Head
    {};

    template<typename Head, typename... Tail>
    struct disjunction<Head, Tail...>:
        conditional_t<Head::value != false, Head, disjunction<Tail...>>
    {};

    ////////////////////////////////////////////////////////////
    // std::negation from C++17

    template<typename T>
    struct negation:
        std::integral_constant<bool, not T::value>
    {};

    ////////////////////////////////////////////////////////////
    // std::remove_cvref from C++20

    template<typename T>
    struct remove_cvref
    {
        using type = std::remove_cv_t<std::remove_reference_t<T>>;
    };

    template<typename T>
    using remove_cvref_t = typename remove_cvref<T>::type;

    ////////////////////////////////////////////////////////////
    // std::is_bounded_array from C++20

    template<typename T>
    struct is_bounded_array:
        std::false_type
    {};

    template<typename T, std::size_t N>
    struct is_bounded_array<T[N]>:
        std::true_type
    {};

    template<typename T>
    constexpr bool is_bounded_array_v = is_bounded_array<T>::value;

    ////////////////////////////////////////////////////////////
    // Type traits to take __int128 into account even when the
    // standard library isn't instrumented but the type is still
    // available:
    // * libstdc++ is instrumented in gnu++ mode only
    // * libc++ is always instrumented
    // * Microsoft STL is never instrumented

#if defined(__SIZEOF_INT128__) && !defined(_LIBCPP_VERSION)
    template<typename T>
    struct is_integral:
        std::is_integral<T>::type
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
        std::is_signed<T>::type
    {};

    template<>
    struct is_signed<__int128_t>:
        std::true_type
    {};

    template<typename T>
    struct is_unsigned:
        std::is_unsigned<T>::type
    {};

    template<>
    struct is_unsigned<__uint128_t>:
        std::true_type
    {};
#else
    using std::is_integral;
    using std::is_signed;
    using std::is_unsigned;
#endif

    ////////////////////////////////////////////////////////////
    // is_specialization_of: check that a given type is a
    // specialization of a given class template, with the caveat
    // that the class template can only have type template
    // parameters
    //
    // See https://wg21.link/P2098R0

    template<typename T, template<typename...> class Template>
    struct is_specialization_of:
        std::false_type
    {};

    template<template<typename...> class Template, typename... Args>
    struct is_specialization_of<Template<Args...>, Template>:
        std::true_type
    {};

    template<typename T, template<typename...> class Template>
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
