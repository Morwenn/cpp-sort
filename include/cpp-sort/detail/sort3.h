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
#ifndef CPPSORT_DETAIL_SORT3_H_
#define CPPSORT_DETAIL_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>

namespace cppsort
{
namespace detail
{
    template<typename FallbackSorter>
    struct sorter_n<3u, FallbackSorter>
    {
        template<typename RandomAccessIterable, typename Compare>
        static auto do_it(RandomAccessIterable& iterable, Compare compare)
            -> void
        {
            using std::swap;

            if (compare(iterable[0u], iterable[1u])) {
                if (compare(iterable[2u], iterable[0u])) {
                    swap(iterable[0u], iterable[2u]);
                }
            } else {
                if (compare(iterable[1u], iterable[2u])) {
                    swap(iterable[0u], iterable[1u]);
                } else {
                    swap(iterable[0u], iterable[2u]);
                }
            }
            if (compare(iterable[2u], iterable[1u])) {
                swap(iterable[1u], iterable[2u]);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_SORT3_H_
