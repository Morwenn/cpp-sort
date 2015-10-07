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
#ifndef CPPSORT_DETAIL_SMALL_ARRAY_SORT14_H_
#define CPPSORT_DETAIL_SMALL_ARRAY_SORT14_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/sorter_facade.h>

namespace cppsort
{
namespace detail
{
    template<typename FallbackSorter>
    struct sorter_n<14u, FallbackSorter>:
        sorter_facade<sorter_n<14u, FallbackSorter>>
    {
        using sorter_facade<sorter_n<14u, FallbackSorter>>::operator();

        template<typename RandomAccessIterator, typename Compare>
        auto operator()(RandomAccessIterator first, RandomAccessIterator, Compare compare) const
            -> void
        {
            using std::swap;

            if (compare(first[1u], first[0u])) {
                swap(first[0u], first[1u]);
            }
            if (compare(first[3u], first[2u])) {
                swap(first[2u], first[3u]);
            }
            if (compare(first[5u], first[4u])) {
                swap(first[4u], first[5u]);
            }
            if (compare(first[7u], first[6u])) {
                swap(first[6u], first[7u]);
            }
            if (compare(first[9u], first[8u])) {
                swap(first[8u], first[9u]);
            }
            if (compare(first[11u], first[10u])) {
                swap(first[10u], first[11u]);
            }
            if (compare(first[13u], first[12u])) {
                swap(first[12u], first[13u]);
            }
            if (compare(first[2u], first[0u])) {
                swap(first[0u], first[2u]);
            }
            if (compare(first[6u], first[4u])) {
                swap(first[4u], first[6u]);
            }
            if (compare(first[10u], first[8u])) {
                swap(first[8u], first[10u]);
            }
            if (compare(first[3u], first[1u])) {
                swap(first[1u], first[3u]);
            }
            if (compare(first[7u], first[5u])) {
                swap(first[5u], first[7u]);
            }
            if (compare(first[11u], first[9u])) {
                swap(first[9u], first[11u]);
            }
            if (compare(first[4u], first[0u])) {
                swap(first[0u], first[4u]);
            }
            if (compare(first[12u], first[8u])) {
                swap(first[8u], first[12u]);
            }
            if (compare(first[5u], first[1u])) {
                swap(first[1u], first[5u]);
            }
            if (compare(first[13u], first[9u])) {
                swap(first[9u], first[13u]);
            }
            if (compare(first[6u], first[2u])) {
                swap(first[2u], first[6u]);
            }
            if (compare(first[7u], first[3u])) {
                swap(first[3u], first[7u]);
            }
            if (compare(first[8u], first[0u])) {
                swap(first[0u], first[8u]);
            }
            if (compare(first[9u], first[1u])) {
                swap(first[1u], first[9u]);
            }
            if (compare(first[10u], first[2u])) {
                swap(first[2u], first[10u]);
            }
            if (compare(first[11u], first[3u])) {
                swap(first[3u], first[11u]);
            }
            if (compare(first[12u], first[4u])) {
                swap(first[4u], first[12u]);
            }
            if (compare(first[13u], first[5u])) {
                swap(first[5u], first[13u]);
            }
            if (compare(first[10u], first[5u])) {
                swap(first[5u], first[10u]);
            }
            if (compare(first[9u], first[6u])) {
                swap(first[6u], first[9u]);
            }
            if (compare(first[12u], first[3u])) {
                swap(first[3u], first[12u]);
            }
            if (compare(first[11u], first[7u])) {
                swap(first[7u], first[11u]);
            }
            if (compare(first[2u], first[1u])) {
                swap(first[1u], first[2u]);
            }
            if (compare(first[8u], first[4u])) {
                swap(first[4u], first[8u]);
            }
            if (compare(first[4u], first[1u])) {
                swap(first[1u], first[4u]);
            }
            if (compare(first[13u], first[7u])) {
                swap(first[7u], first[13u]);
            }
            if (compare(first[8u], first[2u])) {
                swap(first[2u], first[8u]);
            }
            if (compare(first[4u], first[2u])) {
                swap(first[2u], first[4u]);
            }
            if (compare(first[6u], first[5u])) {
                swap(first[5u], first[6u]);
            }
            if (compare(first[10u], first[9u])) {
                swap(first[9u], first[10u]);
            }
            if (compare(first[13u], first[11u])) {
                swap(first[11u], first[13u]);
            }
            if (compare(first[8u], first[3u])) {
                swap(first[3u], first[8u]);
            }
            if (compare(first[12u], first[7u])) {
                swap(first[7u], first[12u]);
            }
            if (compare(first[8u], first[6u])) {
                swap(first[6u], first[8u]);
            }
            if (compare(first[12u], first[10u])) {
                swap(first[10u], first[12u]);
            }
            if (compare(first[5u], first[3u])) {
                swap(first[3u], first[5u]);
            }
            if (compare(first[9u], first[7u])) {
                swap(first[7u], first[9u]);
            }
            if (compare(first[4u], first[3u])) {
                swap(first[3u], first[4u]);
            }
            if (compare(first[6u], first[5u])) {
                swap(first[5u], first[6u]);
            }
            if (compare(first[8u], first[7u])) {
                swap(first[7u], first[8u]);
            }
            if (compare(first[10u], first[9u])) {
                swap(first[9u], first[10u]);
            }
            if (compare(first[12u], first[11u])) {
                swap(first[11u], first[12u]);
            }
            if (compare(first[7u], first[6u])) {
                swap(first[6u], first[7u]);
            }
            if (compare(first[9u], first[8u])) {
                swap(first[8u], first[9u]);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_SMALL_ARRAY_SORT14_H_
