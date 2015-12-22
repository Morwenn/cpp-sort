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
#ifndef CPPSORT_DETAIL_ROTATE_LEFT_H_
#define CPPSORT_DETAIL_ROTATE_LEFT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <utility>

namespace cppsort
{
namespace detail
{
    template<std::size_t N>
    struct rotate_left_n
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator first) const
            -> void
        {
            auto tmp = std::move(first[0u]);
            for (std::size_t i = 0u ; i < N - 1u ; ++i)
            {
                first[i] = std::move(first[i+1u]);
            }
            first[N-1u] = std::move(tmp);
        }
    };

    template<>
    struct rotate_left_n<0u>
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator) const
            -> void
        {}
    };

    template<std::size_t N, typename RandomAccessIterator>
    auto rotate_left(RandomAccessIterator first)
        -> void
    {
        return rotate_left_n<N>{}(first);
    }
}}

#endif // CPPSORT_DETAIL_ROTATE_LEFT_H_
