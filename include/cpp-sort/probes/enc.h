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
#ifndef CPPSORT_PROBES_ENC_H_
#define CPPSORT_PROBES_ENC_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct enc_impl
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
                auto&& proj = utility::as_function(projection);

                // Head an tail of encroaching lists
                std::vector<std::pair<ForwardIterator, ForwardIterator>> lists;

                while (first != last)
                {
                    bool found = false;
                    for (auto& list: lists)
                    {
                        if (not compare(proj(*list.first), proj(*first)))
                        {
                            list.first = first;
                            found = true;
                            break;
                        }

                        if (not compare(proj(*first), proj(*list.second)))
                        {
                            list.second = first;
                            found = true;
                            break;
                        }
                    }

                    if (not found)
                    {
                        // Create a new encroaching list if the element
                        // didn't fit in any of the existing ones
                        lists.emplace_back(first, first);
                    }

                    ++first;
                }

                return lists.size() ? lists.size() - 1 : 0;
            }
        };
    }

    namespace
    {
        constexpr auto&& enc = utility::static_const<
            sorter_facade<detail::enc_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_ENC_H_
