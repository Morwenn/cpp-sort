/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_TYPE_TRAITS_H_
#define CPPSORT_DETAIL_TYPE_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>

namespace cppsort
{
namespace detail
{
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
    // std::is_invocable from C++17

    template<typename T, typename Ret=void, typename=void>
    struct is_invocable_impl:
        std::false_type
    {};

    template<typename Func, typename... Args>
    struct is_invocable_impl<Func(Args...), void, void_t<std::result_of_t<Func(Args...)>>>:
        std::true_type
    {};

    template<typename Func, typename Ret, typename... Args>
    struct is_invocable_impl<Func(Args...), Ret, void_t<std::result_of_t<Func(Args...)>>>:
        std::is_convertible<std::result_of_t<Func(Args...)>, Ret>
    {};

    template<typename T>
    struct is_invocable:
        is_invocable_impl<T, void>
    {};

    template<typename T>
    constexpr bool is_invocable_v = is_invocable<T>::value;

    template<typename T, typename Ret>
    struct is_invocable_r:
        is_invocable_impl<T, Ret>
    {};

    template<typename T, typename Ret>
    constexpr bool is_invocable_r_v = is_invocable_r<T, Ret>::value;

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
        std::conditional_t<Head::value != false, conjunction<Tail...>, Head>
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
        std::conditional_t<Head::value != false, Head, disjunction<Tail...>>
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
}}

#endif // CPPSORT_DETAIL_TYPE_TRAITS_H_
