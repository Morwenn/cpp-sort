/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_UTILITY_DETECTION_H_
#define CPPSORT_UTILITY_DETECTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>

namespace cppsort
{
namespace utility
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

    namespace detail
    {
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
    }

    template<template<typename...> class Op, typename... Args>
    using is_detected = typename detail::detector<nonesuch, void, Op, Args...>::value_t;

    template<template<typename...> class Op, typename... Args>
    using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

    template<typename Default, template<typename...> class Op, typename... Args>
    using detected_or = detail::detector<Default, void, Op, Args...>;

    template<template<typename...> class Op, typename... Args >
    constexpr bool is_detected_v = is_detected<Op, Args...>::value;

    template<typename Default, template<typename...> class Op, typename... Args >
    using detected_or_t = typename detected_or<Default, Op, Args...>::type;

    template<typename Expected, template<typename...> class Op, typename... Args>
    using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

    template<typename Expected, template<typename...> class Op, typename... Args>
    constexpr bool is_detected_exact_v = is_detected_exact<Expected, Op, Args...>::value;

    template<typename To, template<typename...> class Op, typename... Args>
    using is_detected_convertible = std::is_convertible<detected_t<Op, Args...>, To>;

    template<typename To, template<typename...> class Op, typename... Args>
    constexpr bool is_detected_convertible_v = is_detected_convertible<To, Op, Args...>::value;
}}

#endif // CPPSORT_UTILITY_DETECTION_H_
