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
#include <memory>
#include <new>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/count_inversions.h"
#include "../detail/indirect_compare.h"
#include "../detail/iterator_traits.h"
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
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator<Projection, ForwardIterator, Compare>
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

                // Try to allocate enough memory to use a O(n log n) algorithm
                std::unique_ptr<ForwardIterator[]> iterators(new (std::nothrow) ForwardIterator[size]);
                std::unique_ptr<ForwardIterator[]> buffer(new (std::nothrow) ForwardIterator[size]);

                if (iterators != nullptr && buffer != nullptr)
                {
                    ////////////////////////////////////////////////////////////
                    // Use a O(n log n) algorithm if enough memory could be
                    // allocated

                    auto store = iterators.get();
                    for (ForwardIterator it = first ; it != last ; ++it)
                    {
                        *store++ = it;
                    }

                    return cppsort::detail::count_inversions<difference_type>(
                        iterators.get(), iterators.get() + size, buffer.get(),
                        cppsort::detail::indirect_compare<Compare, Projection>(compare, projection)
                    );
                }
                else
                {
                    ////////////////////////////////////////////////////////////
                    // Use a O(n^2) algorithm if not enough memory could be
                    // allocated

                    return cppsort::detail::count_inversions_quadratic(
                        first, last, compare, projection
                    );
                }
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
