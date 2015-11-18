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
#ifndef CPPSORT_DETAIL_BUBBLE_SORT_H_
#define CPPSORT_DETAIL_BUBBLE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
namespace detail
{
    //
    // This sorting algorithm isn't exposed to users of the
    // library, it's only intended to be used as a fallback
    // by other algorithms to sort small collections
    //
    // These recursive algorithms tend to compute the size
    // of the collection, so bubble_sort can use it to have
    // a decreasing bound for forward iterators
    //

    template<typename ForwardIterator, typename Compare, typename Projection>
    void bubble_sort(ForwardIterator first, Compare compare,
                     Projection projection, std::size_t size)
    {
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        while (--size)
        {
            ForwardIterator current = first;
            ForwardIterator next = std::next(current);
            for (std::size_t i = 0 ; i < size ; ++i)
            {
                if (compare(proj(*next), proj(*current)))
                {
                    std::iter_swap(current, next);
                }
                ++next;
                ++current;
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_BUBBLE_SORT_H_
