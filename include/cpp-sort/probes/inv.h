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
#ifndef CPPSORT_PROBES_INV_H_
#define CPPSORT_PROBES_INV_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
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
        struct inv_impl
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

                difference_type count = 0;
                for (auto it1 = first ; it1 != last ; ++it1)
                {
                    auto&& value = proj(*it1);
                    for (auto it2 = std::next(it1) ; it2 != last ; ++it2)
                    {
                        if (compare(proj(*it2), value))
                        {
                            ++count;
                        }
                    }
                }
                return count;
            }
        };
    }

    namespace
    {
        constexpr auto&& inv = cppsort::detail::static_const<
            sorter_facade<detail::inv_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_INV_H_
