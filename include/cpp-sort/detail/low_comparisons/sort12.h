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
#ifndef CPPSORT_DETAIL_LOW_COMPARISONS_SORT12_H_
#define CPPSORT_DETAIL_LOW_COMPARISONS_SORT12_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <utility>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../swap_if.h"

namespace cppsort
{
namespace detail
{
    template<>
    struct low_comparisons_sorter_impl<12u>
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

            low_comparisons_sorter<10u>{}(first+1u, first+11u, compare, projection);
            iter_swap_if(first, first + 11u, compare, projection);

            if (compare(proj(first[1u]), proj(first[0u]))) {
                swap(first[0u], first[1u]);
                if (compare(proj(first[2u]), proj(first[1u]))) {
                    swap(first[1u], first[2u]);
                    if (compare(proj(first[3u]), proj(first[2u]))) {
                        swap(first[2u], first[3u]);
                        if (compare(proj(first[4u]), proj(first[3u]))) {
                            swap(first[3u], first[4u]);
                            if (compare(proj(first[5u]), proj(first[4u]))) {
                                swap(first[4u], first[5u]);
                                if (compare(proj(first[6u]), proj(first[5u]))) {
                                    swap(first[5u], first[6u]);
                                    if (compare(proj(first[7u]), proj(first[6u]))) {
                                        swap(first[6u], first[7u]);
                                        if (compare(proj(first[8u]), proj(first[7u]))) {
                                            swap(first[7u], first[8u]);
                                            if (compare(proj(first[9u]), proj(first[8u]))) {
                                                swap(first[8u], first[9u]);
                                                if (compare(proj(first[10u]), proj(first[9u]))) {
                                                    swap(first[9u], first[10u]);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (compare(proj(first[11u]), proj(first[10u]))) {
                swap(first[10u], first[11u]);
                if (compare(proj(first[10u]), proj(first[9u]))) {
                    swap(first[9u], first[10u]);
                    if (compare(proj(first[9u]), proj(first[8u]))) {
                        swap(first[8u], first[9u]);
                        if (compare(proj(first[8u]), proj(first[7u]))) {
                            swap(first[7u], first[8u]);
                            if (compare(proj(first[7u]), proj(first[6u]))) {
                                swap(first[6u], first[7u]);
                                if (compare(proj(first[6u]), proj(first[5u]))) {
                                    swap(first[5u], first[6u]);
                                    if (compare(proj(first[5u]), proj(first[4u]))) {
                                        swap(first[4u], first[5u]);
                                        if (compare(proj(first[4u]), proj(first[3u]))) {
                                            swap(first[3u], first[4u]);
                                            if (compare(proj(first[3u]), proj(first[2u]))) {
                                                swap(first[2u], first[3u]);
                                                if (compare(proj(first[2u]), proj(first[1u]))) {
                                                    swap(first[1u], first[2u]);
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    };
}}

#endif // CPPSORT_DETAIL_LOW_COMPARISONS_SORT12_H_
