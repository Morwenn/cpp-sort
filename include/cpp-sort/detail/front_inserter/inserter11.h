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
#ifndef CPPSORT_DETAIL_FRONT_INSERTER11_H_
#define CPPSORT_DETAIL_FRONT_INSERTER11_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/utility/as_function.h>
#include "../rotate_left.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct front_inserter_n<11u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare,
            typename Projection
        >
        auto operator()(RandomAccessIterator first, Compare compare, Projection projection) const
            -> void
        {
            auto&& proj = utility::as_function(projection);
            auto&& proj0 = proj(first[0u]);

            if (compare(proj(first[5u]), proj0)) {
                if (compare(proj(first[8u]), proj0)) {
                    if (compare(proj(first[9u]), proj0)) {
                        if (compare(proj(first[10u]), proj0)) {
                            rotate_left<11u>(first);
                        } else {
                            rotate_left<10u>(first);
                        }
                    } else {
                        rotate_left<9u>(first);
                    }
                } else {
                    if (compare(proj(first[7u]), proj0)) {
                        rotate_left<8u>(first);
                    } else {
                        if (compare(proj(first[6u]), proj0)) {
                            rotate_left<7u>(first);
                        } else {
                            rotate_left<6u>(first);
                        }
                    }
                }
            } else {
                if (compare(proj(first[3u]), proj0)) {
                    if (compare(proj(first[4u]), proj0)) {
                        rotate_left<5u>(first);
                    } else {
                        rotate_left<4u>(first);
                    }
                } else {
                    if (compare(proj(first[2u]), proj0)) {
                        rotate_left<3u>(first);
                    } else {
                        if (compare(proj(first[1u]), proj0)) {
                            rotate_left<2u>(first);
                        }
                    }
                }
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_FRONT_INSERTER11_H_
