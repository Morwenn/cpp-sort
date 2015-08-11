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
#ifndef CPPSORT_DETAIL_SORT20_H_
#define CPPSORT_DETAIL_SORT20_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>

namespace cppsort
{
namespace detail
{
    template<typename FallbackSorter>
    struct sorter_n<20u, FallbackSorter>
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
            if (compare(iterable[18u], iterable[2u])) {
                swap(iterable[2u], iterable[18u]);
            }
            if (compare(iterable[19u], iterable[3u])) {
                swap(iterable[3u], iterable[19u]);
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
            if (compare(iterable[18u], iterable[10u])) {
                swap(iterable[10u], iterable[18u]);
            }
            if (compare(iterable[19u], iterable[11u])) {
                swap(iterable[11u], iterable[19u]);
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
            if (compare(iterable[18u], iterable[6u])) {
                swap(iterable[6u], iterable[18u]);
            }
            if (compare(iterable[19u], iterable[7u])) {
                swap(iterable[7u], iterable[19u]);
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
            if (compare(iterable[18u], iterable[14u])) {
                swap(iterable[14u], iterable[18u]);
            }
            if (compare(iterable[19u], iterable[15u])) {
                swap(iterable[15u], iterable[19u]);
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
            if (compare(iterable[18u], iterable[16u])) {
                swap(iterable[16u], iterable[18u]);
            }
            if (compare(iterable[19u], iterable[17u])) {
                swap(iterable[17u], iterable[19u]);
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
            if (compare(iterable[19u], iterable[18u])) {
                swap(iterable[18u], iterable[19u]);
            }
            if (compare(iterable[16u], iterable[1u])) {
                swap(iterable[1u], iterable[16u]);
            }
            if (compare(iterable[18u], iterable[3u])) {
                swap(iterable[3u], iterable[18u]);
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
            if (compare(iterable[18u], iterable[11u])) {
                swap(iterable[11u], iterable[18u]);
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
            if (compare(iterable[18u], iterable[15u])) {
                swap(iterable[15u], iterable[18u]);
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
            if (compare(iterable[18u], iterable[17u])) {
                swap(iterable[17u], iterable[18u]);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_SORT20_H_
