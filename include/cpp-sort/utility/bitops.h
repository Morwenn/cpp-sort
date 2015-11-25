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
#ifndef CPPSORT_UTILITY_BITOPS_H_
#define CPPSORT_UTILITY_BITOPS_H_

namespace cppsort
{
namespace utility
{
    // Returns 2^ceil(log2(n)), assumes n > 0
    template<typename Unsigned>
    auto hyperceil(Unsigned n)
        -> Unsigned
    {
        static constexpr auto bound = std::numeric_limits<Unsigned>::digits / 2;
        for (std::size_t i = 1 ; i <= bound ; i <<= 1)
        {
            n |= (n >> i);
        }
        return n + 1;
    }

    // Returns 2^floor(log2(n)), assumes n > 0
    template<typename Unsigned>
    auto hyperfloor(Unsigned n)
        -> Unsigned
    {
        static constexpr auto bound = std::numeric_limits<Unsigned>::digits / 2;
        for (std::size_t i = 1 ; i <= bound ; i <<= 1)
        {
            n |= (n >> i);
        }
        return n & ~(n >> 1);
    }

    // Returns floor(log2(n)), assumes n > 0
    template<typename Integer>
    auto log2(Integer n)
        -> Integer
    {
        Integer log = 0;
        while (n >>= 1)
        {
            ++log;
        }
        return log;
    }
}}

#endif // CPPSORT_UTILITY_BITOPS_H_
