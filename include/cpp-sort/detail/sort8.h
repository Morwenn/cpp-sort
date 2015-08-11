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
#ifndef CPPSORT_DETAIL_SORT8_H_
#define CPPSORT_DETAIL_SORT8_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>

namespace cppsort
{
namespace detail
{
    template<typename FallbackSorter>
    struct sorter_n<8u, FallbackSorter>
    {
        template<typename RandomAccessIterable, typename Compare>
        static auto do_it(RandomAccessIterable& iterable, Compare compare)
            -> void
        {
            using std::swap;

            if (compare(iterable[1u], iterable[0u])) {
                swap(iterable[0u], iterable[1u]);
            }
            if (compare(iterable[3u], iterable[2u])) {
                swap(iterable[2u], iterable[3u]);
            }
            if (compare(iterable[2u], iterable[0u])) {
                swap(iterable[0u], iterable[2u]);
            }
            if (compare(iterable[3u], iterable[1u])) {
                swap(iterable[1u], iterable[3u]);
            }
            if (compare(iterable[2u], iterable[1u])) {
                swap(iterable[1u], iterable[2u]);
            }
            if (compare(iterable[5u], iterable[4u])) {
                swap(iterable[4u], iterable[5u]);
            }
            if (compare(iterable[7u], iterable[6u])) {
                swap(iterable[6u], iterable[7u]);
            }
            if (compare(iterable[6u], iterable[4u])) {
                swap(iterable[4u], iterable[6u]);
            }
            if (compare(iterable[7u], iterable[5u])) {
                swap(iterable[5u], iterable[7u]);
            }
            if (compare(iterable[6u], iterable[5u])) {
                swap(iterable[5u], iterable[6u]);
            }
            if (compare(iterable[4u], iterable[0u])) {
                swap(iterable[0u], iterable[4u]);
            }
            if (compare(iterable[5u], iterable[1u])) {
                swap(iterable[1u], iterable[5u]);
            }
            if (compare(iterable[4u], iterable[1u])) {
                swap(iterable[1u], iterable[4u]);
            }
            if (compare(iterable[6u], iterable[2u])) {
                swap(iterable[2u], iterable[6u]);
            }
            if (compare(iterable[7u], iterable[3u])) {
                swap(iterable[3u], iterable[7u]);
            }
            if (compare(iterable[6u], iterable[3u])) {
                swap(iterable[3u], iterable[6u]);
            }
            if (compare(iterable[4u], iterable[2u])) {
                swap(iterable[2u], iterable[4u]);
            }
            if (compare(iterable[5u], iterable[3u])) {
                swap(iterable[3u], iterable[5u]);
            }
            if (compare(iterable[4u], iterable[3u])) {
                swap(iterable[3u], iterable[4u]);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_SORT8_H_
