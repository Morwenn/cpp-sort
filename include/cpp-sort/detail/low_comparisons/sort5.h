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
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <cpp-sort/sorter_facade.h>

namespace cppsort
{
    template<>
    struct low_comparisons_sorter<5u>:
        sorter_facade<low_comparisons_sorter<5u>>
    {
        using sorter_facade<low_comparisons_sorter<5u>>::operator();

        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator, Compare compare={}) const
            -> void
        {
            using std::swap;

            if (compare(first[1u], first[0u])) {
                swap(first[0u], first[1u]);
            }
            if (compare(first[3u], first[2u])) {
                swap(first[2u], first[3u]);
            }
            if (compare(first[2u], first[0u])) {
                swap(first[0u], first[2u]);
                swap(first[1u], first[3u]);
            }

            if (compare(first[2u], first[4u])) {
                if (compare(first[4u], first[3u])) {
                    swap(first[3u], first[4u]);
                }
            } else {
                if (compare(first[0u], first[4u])) {
                    auto tmp = std::move(first[2u]);
                    first[2u] = std::move(first[4u]);
                    first[4u] = std::move(first[3u]);
                    first[3u] = std::move(tmp);
                } else {
                    auto tmp = std::move(first[0u]);
                    first[0u] = std::move(first[4u]);
                    first[4u] = std::move(first[3u]);
                    first[3u] = std::move(first[2u]);
                    first[2u] = std::move(tmp);
                }
            }

            if (compare(first[3u], first[1u])) {
                if (compare(first[4u], first[1u])) {
                    auto tmp = std::move(first[1u]);
                    first[1u] = std::move(first[2u]);
                    first[2u] = std::move(first[3u]);
                    first[3u] = std::move(first[4u]);
                    first[4u] = std::move(tmp);
                } else {
                    auto tmp = std::move(first[1u]);
                    first[1u] = std::move(first[2u]);
                    first[2u] = std::move(first[3u]);
                    first[3u] = std::move(tmp);
                }
            } else {
                if (compare(first[2u], first[1u])) {
                    swap(first[1u], first[2u]);
                }
            }
        }
    };
}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_
