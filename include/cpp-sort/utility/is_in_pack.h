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
