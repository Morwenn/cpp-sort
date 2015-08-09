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
#ifndef CPPSORT_SORT_H_
#define CPPSORT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <cpp-sort/sort_n.h>

namespace cppsort
{
    template<
        typename RandomAccessIterable,
        typename Compare = std::less<>
    >
    auto sort(RandomAccessIterable& iterable, Compare compare={})
        -> void
    {
        std::sort(std::begin(iterable), std::end(iterable), compare);
    }

    template<
        typename T,
        std::size_t N,
        typename Compare = std::less<>
    >
    auto sort(std::array<T, N>& array, Compare compare={})
        -> void
    {
        sort_n<N>(std::begin(array), compare);
    }

    template<
        typename T,
        std::size_t N,
        typename Compare = std::less<>
    >
    auto sort(T (&array)[N], Compare compare={})
        -> void
    {
        sort_n<N>(std::begin(array), compare);
    }
}

#endif // CPPSORT_SORT_H_
