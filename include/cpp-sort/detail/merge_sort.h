/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
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
    template<typename ForwardIterator, typename Compare, typename Projection>
    auto merge_sort_impl(ForwardIterator first, difference_type_t<ForwardIterator> size,
                         temporary_buffer<rvalue_type_t<ForwardIterator>>&& buffer,
                         Compare compare, Projection projection,
                         std::forward_iterator_tag tag)
        -> temporary_buffer<rvalue_type_t<ForwardIterator>>
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
                         temporary_buffer<rvalue_type_t<BidirectionalIterator>>&& buffer,
                         Compare compare, Projection projection,
                         std::bidirectional_iterator_tag tag)
        -> temporary_buffer<rvalue_type_t<BidirectionalIterator>>
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

        temporary_buffer<rvalue_type_t<ForwardIterator>> buffer(nullptr);
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

        temporary_buffer<rvalue_type_t<BidirectionalIterator>> buffer(nullptr);
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
