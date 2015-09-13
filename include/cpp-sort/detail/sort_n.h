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
#ifndef CPPSORT_DETAIL_SORT_N_H_
#define CPPSORT_DETAIL_SORT_N_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>

namespace cppsort
{
namespace detail
{
    template<std::size_t N, typename FallbackSorter>
    struct sorter_n
    {
        template<typename RandomAccessIterable, typename Compare>
        static auto do_it(RandomAccessIterable& iterable, Compare compare)
            -> decltype(auto)
        {
            return FallbackSorter{}(iterable, compare);
        }
    };

    template<
        std::size_t N,
        typename FallbackSorter,
        typename RandomAccessIterable,
        typename Compare
    >
    auto sort_n(RandomAccessIterable& iterable, Compare compare)
        -> decltype(auto)
    {
        return sorter_n<N, FallbackSorter>::do_it(iterable, compare);
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
