/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2019 Morwenn
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
#ifndef CPPSORT_DETAIL_SELECTION_SORT_H_
#define CPPSORT_DETAIL_SELECTION_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/utility/iter_move.h>
#include "min_element.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto selection_sort(ForwardIterator first, ForwardIterator last,
                        Compare compare, Projection projection)
        -> void
    {
        using utility::iter_swap;

        using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;

        for (ForwardIterator it = first ; it != last ; ++it) {
            auto min_it = unchecked_min_element(it, last, compare, projection);
            if (std::is_trivially_copyable<rvalue_reference>::value) {
                // If the type is trivially copyable, the potential self-swap
                // should not trigger any issue
                iter_swap(it, min_it);
            } else {
                if (it != min_it) {
                    iter_swap(it, min_it);
                }
            }
        }
    }
}}

#endif // CPPSORT_DETAIL_SELECTION_SORT_H_
