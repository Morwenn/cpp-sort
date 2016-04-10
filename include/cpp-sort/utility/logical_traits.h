/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_UTILITY_LOGICAL_TRAITS_H_
#define CPPSORT_UTILITY_LOGICAL_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Conjunction

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
    // Disjunction

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
    // Negation

    template<typename T>
    struct negation:
        std::integral_constant<bool, not T::value>
    {};
}}

#endif // CPPSORT_UTILITY_LOGICAL_TRAITS_H_
