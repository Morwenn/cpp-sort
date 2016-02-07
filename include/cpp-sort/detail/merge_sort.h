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
#ifndef CPPSORT_DETAIL_MERGE_SORT_H_
#define CPPSORT_DETAIL_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <memory>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/inplace_merge.h>
#include "bubble_sort.h"
#include "insertion_sort.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_sort(BidirectionalIterator first, BidirectionalIterator last,
                    difference_type_t<BidirectionalIterator> size,
                    Compare compare, Projection projection,
                    std::bidirectional_iterator_tag category)
        -> void
    {
        if (size < 40)
        {
            insertion_sort(first, last, compare, projection);
            return;
        }

        // Divide the range into two partitions
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively sort the partitions
        merge_sort(first, middle, size_left, compare, projection, category);
        merge_sort(middle, last, size - size_left, compare, projection, category);

        // Merge the sorted partitions in-place
        utility::inplace_merge(first, middle, last, compare, projection);
    }

    // std::unique_ptr to handle memory allocated with
    // std::get_temporary_buffer
    template<typename T>
    using buffer_ptr = std::unique_ptr<T[], utility::temporary_buffer_deleter>;

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort_impl(ForwardIterator first, difference_type_t<ForwardIterator> size,
                         buffer_ptr<value_type_t<ForwardIterator>>&& buffer,
                         std::ptrdiff_t& buff_size,
                         Compare compare, Projection projection)
        -> buffer_ptr<value_type_t<ForwardIterator>>
    {
        auto&& proj = utility::as_function(projection);

        if (size < 18)
        {
            bubble_sort(first, compare, projection, size);
            return std::move(buffer);
        }

        // Divide the range into two partitions
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively sort the partitions
        buffer = std::move(merge_sort_impl(
            first, size_left,
            std::move(buffer), buff_size,
            compare, projection
        ));
        buffer = std::move(merge_sort_impl(
            middle, size - size_left,
            std::move(buffer), buff_size,
            compare, projection
        ));

        // Shrink the left partition to merge
        auto&& middle_proj = proj(*middle);
        while (first != middle && not compare(middle_proj, proj(*first)))
        {
            ++first;
            --size_left;
        }
        if (first == middle)
        {
            return std::move(buffer);
        }

        // Try to increase the memory buffer if it not big enough
        if (buff_size < size - (size / 2))
        {
            auto new_buffer = std::get_temporary_buffer<value_type_t<ForwardIterator>>(size - (size / 2));
            buffer.reset(new_buffer.first);
            buff_size = new_buffer.second;
        }

        // Merge the sorted partitions in-place
        utility::detail::merge_n_adaptative(first, size_left, middle, size - (size / 2),
                                            buffer.get(), buff_size,
                                            compare, projection);

        return std::move(buffer);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort(ForwardIterator first, ForwardIterator,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection,
                    std::forward_iterator_tag)
        -> void
    {
        if (size < 18)
        {
            bubble_sort(first, compare, projection, size);
            return;
        }

        buffer_ptr<value_type_t<ForwardIterator>> buffer(nullptr);
        std::ptrdiff_t buffer_size = 0;
        merge_sort_impl(first, size,
                        std::move(buffer), buffer_size,
                        compare, projection);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort(ForwardIterator first, ForwardIterator last,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        merge_sort(first, last, size, compare, projection, category{});
    }
}}

#endif // CPPSORT_DETAIL_MERGE_SORT_H_
