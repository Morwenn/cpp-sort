/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
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
#include <type_traits>
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
    // std::unique_ptr to handle memory allocated with
    // std::get_temporary_buffer
    template<typename T>
    using buffer_ptr = std::unique_ptr<remove_cvref_t<T>[], temporary_buffer_deleter>;

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort_impl(ForwardIterator first, difference_type_t<ForwardIterator> size,
                         buffer_ptr<rvalue_reference_t<ForwardIterator>>&& buffer,
                         std::ptrdiff_t& buff_size,
                         Compare compare, Projection projection)
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
        while (first != middle && not comp(middle_proj, proj(*first))) {
            ++first;
            --size_left;
        }
        if (first == middle) {
            return std::move(buffer);
        }

        // Try to increase the memory buffer if it not big enough
        if (buff_size < size - (size / 2)) {
            using rvalue_reference = remove_cvref_t<rvalue_reference_t<ForwardIterator>>;
            auto new_buffer = std::get_temporary_buffer<rvalue_reference>(size - (size / 2));
            buffer.reset(new_buffer.first);
            buff_size = new_buffer.second;
        }

        // Merge the sorted partitions in-place
        merge_n_adaptative(first, size_left, middle, size - (size / 2),
                           buffer.get(), buff_size,
                           std::move(compare), std::move(projection));

        return std::move(buffer);
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_sort_impl(BidirectionalIterator first, BidirectionalIterator last,
                         difference_type_t<BidirectionalIterator> size,
                         buffer_ptr<rvalue_reference_t<BidirectionalIterator>>&& buffer,
                         std::ptrdiff_t& buff_size,
                         Compare compare, Projection projection)
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
            first, middle, size_left,
            std::move(buffer), buff_size,
            compare, projection
        ));
        buffer = std::move(merge_sort_impl(
            middle, last, size - size_left,
            std::move(buffer), buff_size,
            compare, projection
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
        if (buff_size < size_left) {
            using rvalue_reference = remove_cvref_t<rvalue_reference_t<BidirectionalIterator>>;
            auto new_buffer = std::get_temporary_buffer<rvalue_reference>(size_left);
            buffer.reset(new_buffer.first);
            buff_size = new_buffer.second;
        }

        // Merge the sorted partitions in-place
        using comp_ref = std::add_lvalue_reference_t<Compare>;
        inplace_merge_impl<comp_ref>(std::move(first), std::move(middle), std::move(last),
                                     compare, std::move(projection),
                                     size_left, size - (size / 2),
                                     buffer.get(), buff_size);

        return std::move(buffer);
    }

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort(ForwardIterator first, ForwardIterator,
                    difference_type_t<ForwardIterator> size,
                    Compare compare, Projection projection,
                    std::forward_iterator_tag)
        -> void
    {
        if (size < 14) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return;
        }

        buffer_ptr<rvalue_reference_t<ForwardIterator>> buffer(nullptr);
        std::ptrdiff_t buffer_size = 0;
        merge_sort_impl(std::move(first), size,
                        std::move(buffer), buffer_size,
                        std::move(compare), std::move(projection));
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto merge_sort(BidirectionalIterator first, BidirectionalIterator last,
                    difference_type_t<BidirectionalIterator> size,
                    Compare compare, Projection projection,
                    std::bidirectional_iterator_tag)
        -> void
    {
        if (size < 40) {
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return;
        }

        buffer_ptr<rvalue_reference_t<BidirectionalIterator>> buffer(nullptr);
        std::ptrdiff_t buffer_size = 0;
        merge_sort_impl(std::move(first), std::move(last), size,
                        std::move(buffer), buffer_size,
                        std::move(compare), std::move(projection));
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
