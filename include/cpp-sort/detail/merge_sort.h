/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_MERGE_SORT_H_
#define CPPSORT_DETAIL_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/utility/as_function.h>
#include "bubble_sort.h"
#include "inplace_merge.h"
#include "insertion_sort.h"
#include "iterator_traits.h"
#include "memory.h"

namespace cppsort::detail
{
    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto merge_sort_impl(Iterator first, mstd::iter_difference_t<Iterator> size,
                         temporary_buffer<rvalue_type_t<Iterator>>&& buffer,
                         Compare compare, Projection projection)
        -> temporary_buffer<rvalue_type_t<Iterator>>
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
        auto middle = mstd::next(first, size_left);

        // Recursively sort the partitions
        buffer = std::move(merge_sort_impl(
            first, size_left, std::move(buffer),
            compare, projection
        ));
        buffer = std::move(merge_sort_impl(
            middle, size - size_left, std::move(buffer),
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
        buffer.try_grow(size - (size / 2));

        // Merge the sorted partitions in-place
        recmerge(first, size_left, middle, size - (size / 2),
                 buffer.data(), buffer.size(),
                 std::move(compare), std::move(projection));

        return std::move(buffer);
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto merge_sort_impl(Iterator first, Iterator last,
                         mstd::iter_difference_t<Iterator> size,
                         temporary_buffer<rvalue_type_t<Iterator>>&& buffer,
                         Compare compare, Projection projection)
        -> temporary_buffer<rvalue_type_t<Iterator>>
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
        auto middle = mstd::next(first, size_left);

        // Recursively sort the partitions
        buffer = std::move(merge_sort_impl(
            first, middle, size_left, std::move(buffer),
            compare, projection
        ));
        buffer = std::move(merge_sort_impl(
            middle, last, size - size_left, std::move(buffer),
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
        buffer.try_grow(size_left);

        // Merge the sorted partitions in-place
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      size_left, size - (size / 2),
                      buffer.data(), buffer.size());

        return std::move(buffer);
    }

    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto merge_sort(Iterator first, Iterator,
                    mstd::iter_difference_t<Iterator> size,
                    Compare compare, Projection projection)
        -> void
    {
        if (size < 14) {
            bubble_sort(std::move(first), size,
                        std::move(compare), std::move(projection));
            return;
        }

        auto buffer = temporary_buffer<rvalue_type_t<Iterator>>(nullptr);
        merge_sort_impl(std::move(first), size, std::move(buffer),
                        std::move(compare), std::move(projection));
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto merge_sort(Iterator first, Iterator last,
                    mstd::iter_difference_t<Iterator> size,
                    Compare compare, Projection projection)
        -> void
    {
        if (size < 40) {
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return;
        }

        auto buffer = temporary_buffer<rvalue_type_t<Iterator>>(nullptr);
        merge_sort_impl(std::move(first), std::move(last), size, std::move(buffer),
                        std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_DETAIL_MERGE_SORT_H_
