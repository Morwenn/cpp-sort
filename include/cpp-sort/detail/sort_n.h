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
#ifndef CPPSORT_DETAIL_SORT_N_H_
#define CPPSORT_DETAIL_SORT_N_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <functional>

namespace cppsort
{
namespace detail
{
    template<std::size_t N, typename FallbackSorter>
    struct sorter_n
    {
        template<typename RandomAccessIterable, typename Compare>
        static auto do_it(RandomAccessIterable& iterable, Compare compare)
            -> void
        {
            FallbackSorter{}(iterable, compare);
        }
    };

    template<
        std::size_t N,
        typename FallbackSorter,
        typename RandomAccessIterable,
        typename Compare = std::less<>
    >
    auto sort_n(RandomAccessIterable& iterable, Compare compare)
        -> void
    {
        sorter_n<N, FallbackSorter>::do_it(iterable, compare);
    }
}}

// Specializations of sorter_n for some values of N
#include "sort0.h"
#include "sort1.h"
#include "sort2.h"
#include "sort3.h"
#include "sort4.h"
#include "sort5.h"
#include "sort6.h"
#include "sort7.h"
#include "sort8.h"
#include "sort9.h"
#include "sort10.h"
#include "sort11.h"
#include "sort12.h"
#include "sort13.h"
#include "sort14.h"
#include "sort15.h"
#include "sort16.h"
#include "sort17.h"
#include "sort18.h"
#include "sort19.h"
#include "sort20.h"
#include "sort21.h"
#include "sort22.h"
#include "sort23.h"
#include "sort24.h"
#include "sort25.h"
#include "sort26.h"
#include "sort27.h"
#include "sort28.h"
#include "sort29.h"
#include "sort30.h"
#include "sort31.h"
#include "sort32.h"

#endif // CPPSORT_DETAIL_SORT_N_H_
