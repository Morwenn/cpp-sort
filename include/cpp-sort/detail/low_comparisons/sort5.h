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
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../rotate_right.h"
#include "../swap_if.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_comparisons_sorter_impl<5u>
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = std::enable_if_t<is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using std::swap;
            auto&& proj = utility::as_function(projection);

            // Order elements pair-wise

            iter_swap_if(first, first + 1u, compare, projection);
            iter_swap_if(first + 2u, first + 3u, compare, projection);

            // Order pairs of elements by max value

            if (compare(proj(first[3u]), proj(first[1u]))) {
                swap(first[0u], first[2u]);
                swap(first[1u], first[3u]);
            }

            // Merge-insert minimal elements

            if (compare(proj(first[4u]), proj(first[1u]))) {
                // Insert last element in [0, 1, 3]
                if (compare(proj(first[4u]), proj(first[0u]))) {
                    rotate_right<5u>(first);
                } else {
                    rotate_right<4u>(first + 1u);
                }
            } else {
                if (compare(proj(first[4u]), proj(first[3u]))) {
                    rotate_right<3u>(first + 2u);
                } else {
                    // Insert 2 in [0, 1, 3]
                    if (compare(proj(first[2u]), proj(first[0u]))) {
                        rotate_right<3u>(first);
                    } else {
                        iter_swap_if(first + 1u, first + 2u, compare, projection);
                    }
                    return;
                }
            }

            // Insert 3 in [0, 1, 2]
            if (compare(proj(first[3u]), proj(first[1u]))) {
                if (compare(proj(first[3u]), proj(first[0u]))) {
                    rotate_right<4u>(first);
                } else {
                    rotate_right<3u>(first + 1u);
                }
            } else {
                iter_swap_if(first + 2u, first + 3u, compare, projection);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_
