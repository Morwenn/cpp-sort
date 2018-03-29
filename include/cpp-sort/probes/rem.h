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
#ifndef CPPSORT_PROBES_REM_H_
#define CPPSORT_PROBES_REM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"
#include "../detail/upper_bound.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct rem_impl
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
                // Compute Rem as n - longest non-decreasing subsequence,
                // where the LNDS is computed with an altered patience
                // sorting algorithm

                if (first == last || std::next(first) == last) {
                    return 0;
                }

                // The size is only needed at the end to actually compute Rem, but we
                // need to compute it as-we-go when we are not using random-access
                // iterators to avoid making two passes over the sequence, hence a
                // starting value of 0 in this case
                auto size = std::is_base_of<std::random_access_iterator_tag,
                                            cppsort::detail::iterator_category_t<ForwardIterator>
                            >::value ? std::distance(first, last) : 0;

                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                // Top (smaller) elements in patience sorting stacks
                std::vector<ForwardIterator> stack_tops;

                auto deref_compare = [&](const auto& lhs, auto rhs_it) mutable {
                    return comp(lhs, *rhs_it);
                };
                auto deref_proj = [&](const auto& value) mutable -> decltype(auto) {
                    return proj(value);
                };

                while (first != last) {
                    auto it = cppsort::detail::upper_bound(
                        std::begin(stack_tops), std::end(stack_tops),
                        proj(*first), deref_compare, deref_proj);

                    if (it == std::end(stack_tops)) {
                        // The element is bigger than everything else,
                        // create a new "stack" to put it
                        stack_tops.emplace_back(first);
                    } else {
                        // The element is strictly smaller than the top
                        // of a given stack, replace the stack top
                        *it = first;
                    }
                    ++first;

                    if (not std::is_base_of<std::random_access_iterator_tag,
                                            cppsort::detail::iterator_category_t<ForwardIterator>
                        >::value) {
                        // Compute the size as-we-go if iterators are not random-access
                        ++size;
                    }
                }

                return stack_tops.size() ? size - stack_tops.size() : 0;
            }
        };
    }

    namespace
    {
        constexpr auto&& rem = utility::static_const<
            sorter_facade<detail::rem_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_REM_H_
