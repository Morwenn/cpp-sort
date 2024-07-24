/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_INPLACE_MERGE_H_
#define CPPSORT_DETAIL_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/utility/as_function.h>
#include "config.h"
#include "is_sorted_until.h"
#include "iterator_traits.h"
#include "memory.h"
#include "recmerge_bidirectional.h"
#include "recmerge_forward.h"
#include "symmerge.h"

namespace cppsort::detail
{
    ////////////////////////////////////////////////////////////
    // Forward algorithm using recmerge

    template<
        mstd::forward_iterator Iterator,
        typename T,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection,
                       mstd::iter_difference_t<Iterator> len1,
                       mstd::iter_difference_t<Iterator> len2,
                       T* buffer, std::ptrdiff_t buffer_size)
        -> void
    {
        CPPSORT_AUDIT(detail::is_sorted(first, middle, compare, projection));
        CPPSORT_AUDIT(detail::is_sorted(middle, last, compare, projection));
        (void)last;
        recmerge(std::move(first), len1, std::move(middle), len2,
                 buffer, buffer_size,
                 std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Bidirectional algorithm using recmerge

    template<
        mstd::bidirectional_iterator Iterator,
        typename T,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection,
                       mstd::iter_difference_t<Iterator> len1,
                       mstd::iter_difference_t<Iterator> len2,
                       T* buffer, std::ptrdiff_t buffer_size)
        -> void
    {
        CPPSORT_AUDIT(detail::is_sorted(first, middle, compare, projection));
        CPPSORT_AUDIT(detail::is_sorted(middle, last, compare, projection));
        recmerge(std::move(first), std::move(middle), std::move(last),
                 std::move(compare), std::move(projection),
                 len1, len2, buffer, buffer_size);
    }

    ////////////////////////////////////////////////////////////
    // Random-access algorithm using symmerge

    template<
        mstd::random_access_iterator Iterator,
        typename T,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection,
                       mstd::iter_difference_t<Iterator> len1,
                       mstd::iter_difference_t<Iterator> len2,
                       T* buffer, std::ptrdiff_t buffer_size)
        -> void
    {
        CPPSORT_AUDIT(detail::is_sorted(first, middle, compare, projection));
        CPPSORT_AUDIT(detail::is_sorted(middle, last, compare, projection));
        symmerge(first, 0, middle - first, last - first,
                 std::move(compare), std::move(projection),
                 len1, len2, buffer, buffer_size);
    }

    ////////////////////////////////////////////////////////////
    // Overloads allocating a memory buffer if none was passed

    template<
        mstd::forward_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Shrink the problem size on the left side, makes the
        // size computation potentially cheaper
        while (first != middle && not comp(proj(*middle), proj(*first))) {
            ++first;
        }
        if (first == middle) return;

        auto n0 = mstd::distance(first, middle);
        auto n1 = mstd::distance(middle, last);

        auto buffer = temporary_buffer<rvalue_type_t<Iterator>>((std::max)(n0, n1));
        recmerge(std::move(first), n0, std::move(middle), n1,
                 buffer.data(), buffer.size(),
                 std::move(compare), std::move(projection));
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection,
                       mstd::iter_difference_t<Iterator> len1,
                       mstd::iter_difference_t<Iterator> len2)
        -> void
    {
        auto buffer = temporary_buffer<rvalue_type_t<Iterator>>((std::min)(len1, len2));
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buffer.data(), buffer.size());
    }

    template<
        mstd::bidirectional_iterator Iterator,
        typename Compare,
        typename Projection
    >
    auto inplace_merge(Iterator first, Iterator middle, Iterator last,
                       Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Shrink the problem size on the left side, makes the
        // size computation potentially cheaper
        while (first != middle && not comp(proj(*middle), proj(*first))) {
            ++first;
        }
        if (first == middle) return;

        auto len1 = mstd::distance(first, middle);
        auto len2 = mstd::distance(middle, last);

        auto buffer = temporary_buffer<rvalue_type_t<Iterator>>((std::min)(len1, len2));
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buffer.data(), buffer.size());
    }
}

#endif // CPPSORT_DETAIL_INPLACE_MERGE_H_
