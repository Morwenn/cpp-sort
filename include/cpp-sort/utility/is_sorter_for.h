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
#ifndef CPPSORT_UTILITY_IS_SORTER_FOR_H_
#define CPPSORT_UTILITY_IS_SORTER_FOR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename Sorter, typename Iterable>
        using is_sorter_for_t = std::result_of_t<Sorter(Iterable&)>;
    }

    template<typename Sorter, typename Iterable>
    constexpr bool is_sorter_for = is_detected_v<detail::is_sorter_for_t, Sorter, Iterable>;
}}

#endif // CPPSORT_UTILITY_IS_SORTER_FOR_H_

