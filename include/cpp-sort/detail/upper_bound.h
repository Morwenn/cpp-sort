/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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
#ifndef CPPSORT_DETAIL_UPPER_BOUND_H_
#define CPPSORT_DETAIL_UPPER_BOUND_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <cpp-sort/utility/as_function.h>
#include "bitops.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename T,
             typename Compare, typename Projection>
    auto upper_bound(ForwardIterator first, ForwardIterator last, T&& value,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        auto size = std::distance(first, last);
        while (size > 0) {
            ForwardIterator it = first;
            std::advance(it, half(size));
            if (not comp(value, proj(*it))) {
                first = ++it;
                size -= half(size) + 1;
            } else {
                size = half(size);
            }
        }
        return first;
    }
}}

#endif // CPPSORT_DETAIL_UPPER_BOUND_H_
