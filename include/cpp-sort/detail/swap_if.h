/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#ifndef CPPSORT_DETAIL_SWAP_IF_H_
#define CPPSORT_DETAIL_SWAP_IF_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace cppsort
{
namespace detail
{
    template<typename T, typename Compare, typename Projection>
    auto swap_if(T& lhs, T& rhs, Compare compare, Projection projection)
        -> void
    {
        auto&& proj = utility::as_function(projection);

        if (compare(proj(rhs), proj(lhs)))
        {
            using std::swap;
            swap(lhs, rhs);
        }
    }

    template<typename T>
    auto swap_if(T& lhs, T& rhs)
        -> void
    {
        swap_if(lhs, rhs, std::less<>{}, utility::identity{});
    }

    template<
        typename Integer,
        typename = std::enable_if_t<std::is_integral<Integer>::value>
    >
    auto swap_if(Integer& x, Integer& y, std::less<>, utility::identity)
        -> void
    {
        Integer dx = x;
        x = std::min(x, y);
        y ^= dx ^ x;
    }

    template<
        typename Integer,
        typename = std::enable_if_t<std::is_integral<Integer>::value>
    >
    auto swap_if(Integer& x, Integer& y, std::greater<>, utility::identity)
        -> void
    {
        Integer dx = x;
        x = std::max(x, y);
        y ^= dx ^ x;
    }
}}

#endif // CPPSORT_DETAIL_SWAP_IF_H_
