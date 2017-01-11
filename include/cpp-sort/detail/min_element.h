/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#ifndef CPPSORT_DETAIL_MIN_ELEMENT_H_
#define CPPSORT_DETAIL_MIN_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "assume.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto unchecked_min_element(ForwardIterator first, ForwardIterator last,
                               Compare compare, Projection projection)
        -> ForwardIterator
    {
        // Same algorithm as min_element, but assumes that the
        // the input range is never empty
        CPPSORT_ASSUME(first != last);

        auto&& proj = utility::as_function(projection);

        auto min = first;
        while (++first != last) {
            if (compare(proj(*first), proj(*min))) {
                min = first;
            }
        }
        return min;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto min_element(ForwardIterator first, ForwardIterator last,
                     Compare compare, Projection projection)
        -> ForwardIterator
    {
        if (first == last) return last;
        return unchecked_min_element(std::move(first), std::move(last),
                                     std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_MIN_ELEMENT_H_
