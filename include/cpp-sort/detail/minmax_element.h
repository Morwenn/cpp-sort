/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
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

// Range v3 library
//
//  Copyright Eric Niebler 2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//
// Implementation based on the code in libc++
//   http://http://libcxx.llvm.org/

#ifndef CPPSORT_DETAIL_MINMAX_ELEMENT_H_
#define CPPSORT_DETAIL_MINMAX_ELEMENT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "config.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto unchecked_minmax_element(ForwardIterator begin, ForwardIterator end,
                                  Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        // Same as minmax_element, except that it assumes that the collection
        // contains at least two elements
        CPPSORT_ASSUME(begin != end);
        CPPSORT_ASSUME(std::next(begin) != end);

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);
        std::pair<ForwardIterator, ForwardIterator> result{begin, begin};

        if (comp(proj(*begin), proj(*result.first))) {
            result.first = begin;
        } else {
            result.second = begin;
        }

        while (++begin != end) {
            ForwardIterator tmp = begin;
            if (++begin == end) {
                if (comp(proj(*tmp), proj(*result.first))) {
                    result.first = tmp;
                } else if(not comp(proj(*tmp), proj(*result.second))) {
                    result.second = tmp;
                }
                break;
            } else {
                if (comp(proj(*begin), proj(*tmp))) {
                    if (comp(proj(*begin), proj(*result.first))) {
                        result.first = begin;
                    } if (not comp(proj(*tmp), proj(*result.second))) {
                        result.second = tmp;
                    }
                } else {
                    if (comp(proj(*tmp), proj(*result.first))) {
                        result.first = tmp;
                    } if(not comp(proj(*begin), proj(*result.second))) {
                        result.second = begin;
                    }
                }
            }
        }
        return result;
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto minmax_element(ForwardIterator begin, ForwardIterator end,
                        Compare compare, Projection projection)
        -> std::pair<ForwardIterator, ForwardIterator>
    {
        std::pair<ForwardIterator, ForwardIterator> result{begin, begin};
        if (begin == end || std::next(begin) == end) {
            return result;
        }
        return unchecked_minmax_element(std::move(begin), std::move(end),
                                        std::move(compare), std::move(projection));
    }
}}

#endif // CPPSORT_DETAIL_MINMAX_ELEMENT_H_
