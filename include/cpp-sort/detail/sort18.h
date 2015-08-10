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
#ifndef CPPSORT_DETAIL_SORT18_H_
#define CPPSORT_DETAIL_SORT18_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>

namespace cppsort
{
namespace detail
{
    template<typename FallbackSorter>
    struct sorter_n<18u, FallbackSorter>
    {
        template<typename RandomAccessIterable, typename Compare>
        static auto do_it(RandomAccessIterable& iterable, Compare compare)
            -> void
        {
            using std::swap;

            if (compare(iterable[16u], iterable[0u])) {
                swap(iterable[0u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[1u])) {
                swap(iterable[1u], iterable[17u]);
            }
            if (compare(iterable[8u], iterable[0u])) {
                swap(iterable[0u], iterable[8u]);
            }
            if (compare(iterable[9u], iterable[1u])) {
                swap(iterable[1u], iterable[9u]);
            }
            if (compare(iterable[10u], iterable[2u])) {
                swap(iterable[2u], iterable[10u]);
            }
            if (compare(iterable[11u], iterable[3u])) {
                swap(iterable[3u], iterable[11u]);
            }
            if (compare(iterable[12u], iterable[4u])) {
                swap(iterable[4u], iterable[12u]);
            }
            if (compare(iterable[13u], iterable[5u])) {
                swap(iterable[5u], iterable[13u]);
            }
            if (compare(iterable[14u], iterable[6u])) {
                swap(iterable[6u], iterable[14u]);
            }
            if (compare(iterable[15u], iterable[7u])) {
                swap(iterable[7u], iterable[15u]);
            }
            if (compare(iterable[16u], iterable[8u])) {
                swap(iterable[8u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[9u])) {
                swap(iterable[9u], iterable[17u]);
            }
            if (compare(iterable[4u], iterable[0u])) {
                swap(iterable[0u], iterable[4u]);
            }
            if (compare(iterable[5u], iterable[1u])) {
                swap(iterable[1u], iterable[5u]);
            }
            if (compare(iterable[6u], iterable[2u])) {
                swap(iterable[2u], iterable[6u]);
            }
            if (compare(iterable[7u], iterable[3u])) {
                swap(iterable[3u], iterable[7u]);
            }
            if (compare(iterable[12u], iterable[8u])) {
                swap(iterable[8u], iterable[12u]);
            }
            if (compare(iterable[13u], iterable[9u])) {
                swap(iterable[9u], iterable[13u]);
            }
            if (compare(iterable[14u], iterable[10u])) {
                swap(iterable[10u], iterable[14u]);
            }
            if (compare(iterable[15u], iterable[11u])) {
                swap(iterable[11u], iterable[15u]);
            }
            if (compare(iterable[16u], iterable[4u])) {
                swap(iterable[4u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[5u])) {
                swap(iterable[5u], iterable[17u]);
            }
            if (compare(iterable[8u], iterable[4u])) {
                swap(iterable[4u], iterable[8u]);
            }
            if (compare(iterable[9u], iterable[5u])) {
                swap(iterable[5u], iterable[9u]);
            }
            if (compare(iterable[10u], iterable[6u])) {
                swap(iterable[6u], iterable[10u]);
            }
            if (compare(iterable[11u], iterable[7u])) {
                swap(iterable[7u], iterable[11u]);
            }
            if (compare(iterable[16u], iterable[12u])) {
                swap(iterable[12u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[13u])) {
                swap(iterable[13u], iterable[17u]);
            }
            if (compare(iterable[2u], iterable[0u])) {
                swap(iterable[0u], iterable[2u]);
            }
            if (compare(iterable[3u], iterable[1u])) {
                swap(iterable[1u], iterable[3u]);
            }
            if (compare(iterable[6u], iterable[4u])) {
                swap(iterable[4u], iterable[6u]);
            }
            if (compare(iterable[7u], iterable[5u])) {
                swap(iterable[5u], iterable[7u]);
            }
            if (compare(iterable[10u], iterable[8u])) {
                swap(iterable[8u], iterable[10u]);
            }
            if (compare(iterable[11u], iterable[9u])) {
                swap(iterable[9u], iterable[11u]);
            }
            if (compare(iterable[14u], iterable[12u])) {
                swap(iterable[12u], iterable[14u]);
            }
            if (compare(iterable[15u], iterable[13u])) {
                swap(iterable[13u], iterable[15u]);
            }
            if (compare(iterable[16u], iterable[2u])) {
                swap(iterable[2u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[3u])) {
                swap(iterable[3u], iterable[17u]);
            }
            if (compare(iterable[8u], iterable[2u])) {
                swap(iterable[2u], iterable[8u]);
            }
            if (compare(iterable[9u], iterable[3u])) {
                swap(iterable[3u], iterable[9u]);
            }
            if (compare(iterable[12u], iterable[6u])) {
                swap(iterable[6u], iterable[12u]);
            }
            if (compare(iterable[13u], iterable[7u])) {
                swap(iterable[7u], iterable[13u]);
            }
            if (compare(iterable[16u], iterable[10u])) {
                swap(iterable[10u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[11u])) {
                swap(iterable[11u], iterable[17u]);
            }
            if (compare(iterable[4u], iterable[2u])) {
                swap(iterable[2u], iterable[4u]);
            }
            if (compare(iterable[5u], iterable[3u])) {
                swap(iterable[3u], iterable[5u]);
            }
            if (compare(iterable[8u], iterable[6u])) {
                swap(iterable[6u], iterable[8u]);
            }
            if (compare(iterable[9u], iterable[7u])) {
                swap(iterable[7u], iterable[9u]);
            }
            if (compare(iterable[12u], iterable[10u])) {
                swap(iterable[10u], iterable[12u]);
            }
            if (compare(iterable[13u], iterable[11u])) {
                swap(iterable[11u], iterable[13u]);
            }
            if (compare(iterable[16u], iterable[14u])) {
                swap(iterable[14u], iterable[16u]);
            }
            if (compare(iterable[17u], iterable[15u])) {
                swap(iterable[15u], iterable[17u]);
            }
            if (compare(iterable[1u], iterable[0u])) {
                swap(iterable[0u], iterable[1u]);
            }
            if (compare(iterable[3u], iterable[2u])) {
                swap(iterable[2u], iterable[3u]);
            }
            if (compare(iterable[5u], iterable[4u])) {
                swap(iterable[4u], iterable[5u]);
            }
            if (compare(iterable[7u], iterable[6u])) {
                swap(iterable[6u], iterable[7u]);
            }
            if (compare(iterable[9u], iterable[8u])) {
                swap(iterable[8u], iterable[9u]);
            }
            if (compare(iterable[11u], iterable[10u])) {
                swap(iterable[10u], iterable[11u]);
            }
            if (compare(iterable[13u], iterable[12u])) {
                swap(iterable[12u], iterable[13u]);
            }
            if (compare(iterable[15u], iterable[14u])) {
                swap(iterable[14u], iterable[15u]);
            }
            if (compare(iterable[17u], iterable[16u])) {
                swap(iterable[16u], iterable[17u]);
            }
            if (compare(iterable[16u], iterable[1u])) {
                swap(iterable[1u], iterable[16u]);
            }
            if (compare(iterable[8u], iterable[1u])) {
                swap(iterable[1u], iterable[8u]);
            }
            if (compare(iterable[10u], iterable[3u])) {
                swap(iterable[3u], iterable[10u]);
            }
            if (compare(iterable[12u], iterable[5u])) {
                swap(iterable[5u], iterable[12u]);
            }
            if (compare(iterable[14u], iterable[7u])) {
                swap(iterable[7u], iterable[14u]);
            }
            if (compare(iterable[16u], iterable[9u])) {
                swap(iterable[9u], iterable[16u]);
            }
            if (compare(iterable[4u], iterable[1u])) {
                swap(iterable[1u], iterable[4u]);
            }
            if (compare(iterable[6u], iterable[3u])) {
                swap(iterable[3u], iterable[6u]);
            }
            if (compare(iterable[8u], iterable[5u])) {
                swap(iterable[5u], iterable[8u]);
            }
            if (compare(iterable[10u], iterable[7u])) {
                swap(iterable[7u], iterable[10u]);
            }
            if (compare(iterable[12u], iterable[9u])) {
                swap(iterable[9u], iterable[12u]);
            }
            if (compare(iterable[14u], iterable[11u])) {
                swap(iterable[11u], iterable[14u]);
            }
            if (compare(iterable[16u], iterable[13u])) {
                swap(iterable[13u], iterable[16u]);
            }
            if (compare(iterable[2u], iterable[1u])) {
                swap(iterable[1u], iterable[2u]);
            }
            if (compare(iterable[4u], iterable[3u])) {
                swap(iterable[3u], iterable[4u]);
            }
            if (compare(iterable[6u], iterable[5u])) {
                swap(iterable[5u], iterable[6u]);
            }
            if (compare(iterable[8u], iterable[7u])) {
                swap(iterable[7u], iterable[8u]);
            }
            if (compare(iterable[10u], iterable[9u])) {
                swap(iterable[9u], iterable[10u]);
            }
            if (compare(iterable[12u], iterable[11u])) {
                swap(iterable[11u], iterable[12u]);
            }
            if (compare(iterable[14u], iterable[13u])) {
                swap(iterable[13u], iterable[14u]);
            }
            if (compare(iterable[16u], iterable[15u])) {
                swap(iterable[15u], iterable[16u]);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_SORT18_H_
