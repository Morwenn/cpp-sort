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
#ifndef CPPSORT_FIXED_LOW_MOVES_SORTER_H_
#define CPPSORT_FIXED_LOW_MOVES_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <tuple>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/minmax_element.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<std::size_t N>
    struct low_moves_sorter;

    namespace detail
    {
        template<std::size_t N>
        struct low_moves_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<is_projection_iterator<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                RandomAccessIterator min, max;
                std::tie(min, max) = minmax_element(first, last--, compare, projection);

                if (max == first && min == last)
                {
                    if (min == max) return;
                    std::iter_swap(min, max);
                }
                else if (max == first)
                {
                    if (last != max)
                    {
                        std::iter_swap(last, max);
                    }
                    if (first != min)
                    {
                        std::iter_swap(first, min);
                    }
                }
                else
                {
                    if (first != min)
                    {
                        std::iter_swap(first, min);
                    }
                    if (last != max)
                    {
                        std::iter_swap(last, max);
                    }
                }
                low_moves_sorter<N-2u>{}(++first, last, compare, projection);
            }
        };
    }

    template<std::size_t N>
    struct low_moves_sorter:
        sorter_facade<detail::low_moves_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<low_moves_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        static constexpr bool is_stable = false;
    };
}

// Specializations of low_moves_sorter for some values of N
#include "../detail/low_moves/sort0.h"
#include "../detail/low_moves/sort1.h"
#include "../detail/low_moves/sort2.h"
#include "../detail/low_moves/sort3.h"
#include "../detail/low_moves/sort4.h"

#endif // CPPSORT_FIXED_LOW_MOVES_SORTER_H_
