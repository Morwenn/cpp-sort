/*
 * Copyright (C) 2015 Morwenn
 *
 * array_sort is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * array_sort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not,
 * see <http://www.gnu.org/licenses/>.
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

    namespace detail
    {
        template<typename...>
        struct voider
        {
            using type = void;
        };
    }

    template<typename... TT>
    using void_t = typename detail::voider<TT...>::type;

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
