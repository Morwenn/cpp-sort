/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_PROBES_REM_H_
#define CPPSORT_PROBES_REM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/static_const.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct rem_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator<Projection, RandomAccessIterator, Compare>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> typename std::iterator_traits<RandomAccessIterator>::difference_type
            {
                using difference_type = typename std::iterator_traits<RandomAccessIterator>::difference_type;
                auto&& proj = utility::as_function(projection);

                if (std::distance(first, last) < 2)
                {
                    return 0;
                }

                // Size of the longest ascending subsequence
                difference_type max_size = 0;
                // Beginning of the current subsequence
                auto begin = first;

                auto current = first;
                auto next = std::next(first);
                while (true)
                {
                    while (next != last && compare(proj(*current), proj(*next)))
                    {
                        ++current;
                        ++next;
                    }

                    max_size = std::max(
                        max_size,
                        std::distance(begin, next)
                    );

                    if (next == last) break;
                    ++current;
                    ++next;
                    begin = current;
                }

                return std::distance(first, last) - max_size;
            }
        };
    }

    namespace
    {
        constexpr auto&& rem = cppsort::detail::static_const<
            sorter_facade<detail::rem_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_REM_H_
