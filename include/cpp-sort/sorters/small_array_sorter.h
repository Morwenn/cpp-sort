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
#ifndef CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_
#define CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/is_in_pack.h>
#include "../detail/sort_n.h"

namespace cppsort
{
    template<typename...>
    struct small_array_sorter;

    template<
        typename Sorter,
        std::size_t... Indices
    >
    struct small_array_sorter<Sorter, std::index_sequence<Indices...>>
    {
        template<
            typename RandomAccessIterable,
            typename Compare = std::less<>
        >
        auto operator()(RandomAccessIterable& iterable, Compare compare={}) const
            -> void
        {
            Sorter{}(iterable, compare);
        }

        template<
            typename T,
            std::size_t N,
            typename Compare = std::less<>,
            typename = std::enable_if_t<utility::is_in_pack<N, Indices...>>
        >
        auto operator()(std::array<T, N>& array, Compare compare={}) const
            -> void
        {
            detail::sort_n<N, Sorter>(array, compare);
        }

        template<
            typename T,
            std::size_t N,
            typename Compare = std::less<>,
            typename = std::enable_if_t<utility::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Compare compare={}) const
            -> void
        {
            detail::sort_n<N, Sorter>(array, compare);
        }
    };

    template<typename Sorter>
    struct small_array_sorter<Sorter>:
        small_array_sorter<Sorter, std::make_index_sequence<33u>>
    {};
}

#endif // CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_
