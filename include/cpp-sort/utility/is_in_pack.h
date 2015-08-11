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
#ifndef CPPSORT_UTILITY_IS_IN_PACK_H_
#define CPPSORT_UTILITY_IS_IN_PACK_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <type_traits>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<std::size_t Value, std::size_t... Values>
        struct is_in_pack_impl;

        template<
            std::size_t Value,
            std::size_t Head,
            std::size_t... Tail
        >
        struct is_in_pack_impl<Value, Head, Tail...>:
            std::conditional_t<
                Value == Head,
                std::true_type,
                is_in_pack_impl<Value, Tail...>
            >
        {};

        template<std::size_t Value>
        struct is_in_pack_impl<Value>:
            std::false_type
        {};
    }

    template<std::size_t Value, std::size_t... Values>
    constexpr bool is_in_pack = detail::is_in_pack_impl<Value, Values...>::value;
}}

#endif // CPPSORT_UTILITY_IS_IN_PACK_H_
