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
#ifndef CPPSORT_DETAIL_BUBBLE_SORT_H_
#define CPPSORT_DETAIL_BUBBLE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    //
    // This sorting algorithm isn't exposed to users of the
    // library, it's only intended to be used as a fallback
    // by other stable algorithms to sort small collections
    //
    // These recursive algorithms tend to compute the size
    // of the collection, so bubble_sort can use it to have
    // a decreasing bound for forward iterators
    //

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto bubble_sort(ForwardIterator first, difference_type_t<ForwardIterator> size,
                     Compare compare, Projection projection)
        -> void
    {
        if (size < 2) return;

        using utility::iter_swap;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        while (--size)
        {
            ForwardIterator current = first;
            ForwardIterator next = std::next(current);
            for (difference_type_t<ForwardIterator> i = 0 ; i < size ; ++i)
            {
                if (comp(proj(*next), proj(*current)))
                {
                    iter_swap(current, next);
                }
                ++next;
                ++current;
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_BUBBLE_SORT_H_
