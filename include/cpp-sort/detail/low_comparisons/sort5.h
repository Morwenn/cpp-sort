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
            typename = std::enable_if_t<is_projection_iterator<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator first, RandomAccessIterator,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            using std::swap;
            auto&& proj = utility::as_function(projection);

            swap_if(first[0u], first[1u], compare, projection);
            swap_if(first[2u], first[3u], compare, projection);
            if (compare(proj(first[2u]), proj(first[0u]))) {
                swap(first[0u], first[2u]);
                swap(first[1u], first[3u]);
            }

            if (compare(proj(first[2u]), proj(first[4u]))) {
                swap_if(first[3u], first[4u], compare, projection);
            } else {
                if (compare(proj(first[0u]), proj(first[4u]))) {
                    auto tmp = std::move(first[2u]);
                    first[2u] = std::move(first[4u]);
                    first[4u] = std::move(first[3u]);
                    first[3u] = std::move(tmp);
                } else {
                    auto tmp = std::move(first[0u]);
                    first[0u] = std::move(first[4u]);
                    first[4u] = std::move(first[3u]);
                    first[3u] = std::move(first[2u]);
                    first[2u] = std::move(tmp);
                }
            }

            if (compare(proj(first[3u]), proj(first[1u]))) {
                if (compare(proj(first[4u]), proj(first[1u]))) {
                    auto tmp = std::move(first[1u]);
                    first[1u] = std::move(first[2u]);
                    first[2u] = std::move(first[3u]);
                    first[3u] = std::move(first[4u]);
                    first[4u] = std::move(tmp);
                } else {
                    auto tmp = std::move(first[1u]);
                    first[1u] = std::move(first[2u]);
                    first[2u] = std::move(first[3u]);
                    first[3u] = std::move(tmp);
                }
            } else {
                swap_if(first[1u], first[2u], compare, projection);
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT5_H_
