/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#ifndef CPPSORT_DETAIL_IS_P_SORTED_H_
#define CPPSORT_DETAIL_IS_P_SORTED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/as_function.h>
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto is_p_sorted(RandomAccessIterator first, RandomAccessIterator last,
                     difference_type_t<RandomAccessIterator> p,
                     Compare compare, Projection projection)
        -> bool
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        for (auto it1 = first + p ; it1 != last ; ++it1) {
            auto&& value = proj(*it1);
            for (auto it2 = first ; it2 != it1 - p ; ++it2) {
                if (comp(value, proj(*it2))) {
                    return false;
                }
            }
        }
        return true;
    }
}}

#endif // CPPSORT_DETAIL_IS_P_SORTED_H_
