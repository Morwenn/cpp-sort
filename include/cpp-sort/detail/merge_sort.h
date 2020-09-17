/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "bubble_sort.h"
#include "inplace_merge.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "memory.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename T>
    using buffer_ptr = temporary_buffer<remove_cvref_t<T>>;

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort_impl(ForwardIterator first, difference_type_t<ForwardIterator> size,
                         buffer_ptr<rvalue_reference_t<ForwardIterator>>&& buffer,
                         Compare compare, Projection projection,
                         std::forward_iterator_tag tag)
        -> buffer_ptr<rvalue_reference_t<ForwardIterator>>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (size < 14) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return std::move(buffer);
        }

        // Divide the range into two partitions
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively sort the partitions
        buffer = std::move(merge_sort_impl(
            first, size_left, std::move(buffer),
            compare, projection, tag
        ));
        buffer = std::move(merge_sort_impl(
            middle, size - size_left, std::move(buffer),
            compare, projection, tag
        ));

        // Shrink the left partition to merge
        auto&& middle_proj = proj(*middle);
        while (first != middle && not comp(middle_proj, proj(*first))) {
            ++first;
            --size_left;
        }
        if (first == middle) {
            return std::move(buffer);
        }

        // Try to increase the memory buffer if it not big enough
        buffer.try_grow(size - (size / 2));

        // Merge the sorted partitions in-place
        recmerge(first, size_left, middle, size - (size / 2),
                 buffer.data(), buffer.size(),
                 std::move(compare), std::move(projection), tag);

        return std::move(buffer);
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_sort_impl(BidirectionalIterator first, BidirectionalIterator last,
                         difference_type_t<BidirectionalIterator> size,
                         buffer_ptr<rvalue_reference_t<BidirectionalIterator>>&& buffer,
                         Compare compare, Projection projection,
                         std::bidirectional_iterator_tag tag)
        -> buffer_ptr<rvalue_reference_t<BidirectionalIterator>>
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        if (size < 40) {
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return std::move(buffer);
        }

        // Divide the range into two partitions
        auto size_left = size / 2;
        auto middle = std::next(first, size_left);

        // Recursively sort the partitions
        buffer = std::move(merge_sort_impl(
            first, middle, size_left, std::move(buffer),
            compare, projection, tag
        ));
        buffer = std::move(merge_sort_impl(
            middle, last, size - size_left, std::move(buffer),
            compare, projection, tag
        ));

        // Shrink the left partition to merge
        auto&& middle_proj = proj(*middle);
        while (first != middle && not comp(middle_proj, proj(*first))) {
            ++first;
            --size_left;
        }
        if (first == middle) {
            return std::move(buffer);
        }

        // Try to increase the memory buffer if it not big enough
        buffer.try_grow(size_left);

        // Merge the sorted partitions in-place
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      size_left, size - (size / 2),
                      buffer.data(), buffer.size());

        return std::move(buffer);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort(ForwardIterator first, ForwardIterator,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection,
                    std::forward_iterator_tag tag)
        -> void
    {
        if (size < 14) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return;
        }

        buffer_ptr<rvalue_reference_t<ForwardIterator>> buffer(nullptr);
        merge_sort_impl(std::move(first), size, std::move(buffer),
                        std::move(compare), std::move(projection), tag);
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_sort(BidirectionalIterator first, BidirectionalIterator last,
                    difference_type_t<BidirectionalIterator> size,
                    Compare compare, Projection projection,
                    std::bidirectional_iterator_tag tag)
        -> void
    {
        if (size < 40) {
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return;
        }

        buffer_ptr<rvalue_reference_t<BidirectionalIterator>> buffer(nullptr);
        merge_sort_impl(std::move(first), std::move(last), size, std::move(buffer),
                        std::move(compare), std::move(projection), tag);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort(ForwardIterator first, ForwardIterator last,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        merge_sort(std::move(first), std::move(last), size,
                   std::move(compare), std::move(projection),
                   category{});
    }
}}

#endif // CPPSORT_DETAIL_MERGE_SORT_H_
