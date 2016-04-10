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
#ifndef CPPSORT_PROBES_MAX_H_
#define CPPSORT_PROBES_MAX_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/indirect_compare.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct max_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;

                auto size = std::distance(first, last);
                if (size < 2)
                {
                    return 0;
                }

                ////////////////////////////////////////////////////////////
                // Indirectly sort the iterators

                // Copy the iterators in a vector
                std::vector<ForwardIterator> iterators;
                iterators.reserve(size);
                for (ForwardIterator it = first ; it != last ; ++it)
                {
                    iterators.push_back(it);
                }

                // Sort the iterators on pointed values
                pdq_sorter{}(
                    iterators,
                    cppsort::detail::indirect_compare<Compare, Projection>(compare, projection)
                );

                ////////////////////////////////////////////////////////////
                // Maximum distance an element has to travel in order to
                // reach its sorted position

                difference_type max_dist = 0;
                for (difference_type i = 0 ; i < size ; ++i)
                {
                    auto pos = std::distance(first, iterators[i]);
                    max_dist = std::max(std::abs(pos - i), max_dist);
                }
                return max_dist;
            }
        };
    }

    namespace
    {
        constexpr auto&& max = utility::static_const<
            sorter_facade<detail::max_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_MAX_H_
