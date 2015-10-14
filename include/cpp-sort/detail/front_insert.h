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
#ifndef CPPSORT_DETAIL_FRONT_INSERT_H_
#define CPPSORT_DETAIL_FRONT_INSERT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>

namespace cppsort
{
namespace detail
{
    //
    // Family of fixed-size functions which will try to insert
    // *first in the rest of the collection, assuming that the
    // following elements are sorted with compare
    //
    // When there are enough elements, binary search is used to
    // locate the position where to insert *first
    //

    template<std::size_t N>
    struct front_inserter_n;

    template<
        std::size_t N,
        typename RandomAccessIterator,
        typename Compare
    >
    auto front_insert(RandomAccessIterator first, Compare compare)
        -> decltype(auto)
    {
        return front_inserter_n<N>{}(first, compare);
    }
}}

// Specializations of low_moves_sorter_n for some values of N
#include "front_inserter/inserter0.h"
#include "front_inserter/inserter1.h"
#include "front_inserter/inserter2.h"
#include "front_inserter/inserter3.h"
#include "front_inserter/inserter4.h"
#include "front_inserter/inserter5.h"
#include "front_inserter/inserter6.h"

#endif // CPPSORT_DETAIL_FRONT_INSERT_H_
