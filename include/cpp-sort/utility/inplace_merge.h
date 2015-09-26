/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_UTILITY_INPLACE_MERGE_H_
#define CPPSORT_UTILITY_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <utility>

namespace cppsort
{
namespace utility
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare>
        auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, Compare compare,
                           std::forward_iterator_tag)
            -> void
        {
            using value_type = typename std::iterator_traits<ForwardIterator>::value_type;

            // Shrink the problem size on the left side
            while (compare(*first, *middle))
            {
                ++first;
            }

            // Try to allocate a temporary buffer and make sure
            // it will always be properly deleted
            auto size = std::distance(first, last);
            std::unique_ptr<value_type[]> buffer(new (std::nothrow) value_type[size]);

            // If there is enough memory, use the buffer for the merge
            if (buffer != nullptr)
            {
                std::merge(first, middle, middle, last, buffer.get(), compare);
                std::move(buffer.get(), buffer.get() + size, first);
            }
            else // Hand-made in-place merge algorithm
            {
                ForwardIterator insertion_point = middle;
                for (; first != middle ; ++first)
                {
                    if (compare(*middle, *first))
                    {
                        // *first should be in the right partition
                        value_type tmp = std::move(*first);

                        // Put *right in place
                        *first = std::move(*middle);

                        // Look for the place where to insert tmp
                        ForwardIterator next = std::next(insertion_point);

                        // Move everything smaller than tmp to the left
                        std::move(std::next(middle), next, middle);
                        while (next != last && compare(*next, tmp))
                        {
                            *insertion_point++ = std::move(*next++);
                        }

                        // Insert tmp in the right place
                        *insertion_point = std::move(tmp);
                    }
                }
            }
        }

        template<typename BidirectionalIterator, typename Compare>
        auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                           BidirectionalIterator last, Compare compare,
                           std::bidirectional_iterator_tag)
            -> void
        {
            std::inplace_merge(first, middle, last, compare);
        }
    }

    template<
        typename ForwardIterator,
        typename Compare = std::less<>
    >
    auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                       ForwardIterator last, Compare compare={})
        -> void
    {
        using category = typename std::iterator_traits<ForwardIterator>::iterator_category;
        detail::inplace_merge(first, middle, last, compare, category{});
    }
}}

#endif // CPPSORT_UTILITY_INPLACE_MERGE_H_
