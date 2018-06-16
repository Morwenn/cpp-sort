/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#ifndef CPPSORT_PROBES_HAM_H_
#define CPPSORT_PROBES_HAM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/indirect_compare.h"
#include "../detail/iterator_traits.h"
#include "../detail/pdqsort.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct ham_impl
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
                if (size < 2) {
                    return 0;
                }

                ////////////////////////////////////////////////////////////
                // Indirectly sort the iterators

                // Copy the iterators in a vector
                std::vector<ForwardIterator> iterators;
                iterators.reserve(size);
                for (ForwardIterator it = first ; it != last ; ++it) {
                    iterators.push_back(it);
                }

                // Sort the iterators on pointed values
                pdqsort(
                    iterators.begin(), iterators.end(),
                    cppsort::detail::indirect_compare<Compare, Projection>(std::move(compare),
                                                                           std::move(projection)),
                    utility::identity{}
                );

                ////////////////////////////////////////////////////////////
                // Count the number of values not in place

                difference_type count = 0;
                for (auto&& it: iterators) {
                    if (it != first) {
                        ++count;
                    }
                    ++first;
                }
                return count;
            }
        };
    }

    namespace
    {
        constexpr auto&& ham = utility::static_const<
            sorter_facade<detail::ham_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_HAM_H_
