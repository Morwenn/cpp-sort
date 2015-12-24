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
#ifndef CPPSORT_DETAIL_ITER_SORT3_H_
#define CPPSORT_DETAIL_ITER_SORT3_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    template<typename Iterator, typename Compare, typename Projection>
    auto iter_sort3(Iterator a, Iterator b, Iterator c,
                    Compare compare, Projection projection)
        -> void
    {
        using std::swap;
        auto&& proj = utility::as_function(projection);

        if (compare(proj(*b), proj(*a))) {
            if (compare(proj(*c), proj(*a))) {
                if (compare(proj(*c), proj(*b))) {
                    swap(*a, *c);
                } else {
                    auto tmp = std::move(*a);
                    *a = std::move(*b);
                    *b = std::move(*c);
                    *c = std::move(tmp);
                }
            } else {
                swap(*a, *b);
            }
        } else {
            if (compare(proj(*c), proj(*b))) {
                if (compare(proj(*c), proj(*a))) {
                    auto tmp = std::move(*c);
                    *c = std::move(*b);
                    *b = std::move(*a);
                    *a = std::move(tmp);
                } else {
                    swap(*b, *c);
                }
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_ITER_SORT3_H_
