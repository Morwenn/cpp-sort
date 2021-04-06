/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_INPLACE_MERGE_H_
#define CPPSORT_DETAIL_INPLACE_MERGE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include "iterator_traits.h"
#include "memory.h"
#include "recmerge_bidirectional.h"
#include "recmerge_forward.h"
#include "symmerge.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Forward algorithm using recmerge

    template<typename ForwardIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator,
                       Compare compare, Projection projection,
                       difference_type_t<ForwardIterator> len1,
                       difference_type_t<ForwardIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size,
                       std::forward_iterator_tag)
        -> void
    {
        recmerge(std::move(first), len1, std::move(middle), len2,
                 buff, buff_size,
                 std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // Bidirectional algorithm using recmerge

    template<typename BidirectionalIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<BidirectionalIterator> len1,
                       difference_type_t<BidirectionalIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size,
                       std::bidirectional_iterator_tag tag)
        -> void
    {
        recmerge(std::move(first), std::move(middle), std::move(last),
                 std::move(compare), std::move(projection),
                 len1, len2, buff, buff_size, tag);
    }

    ////////////////////////////////////////////////////////////
    // Random-access algorithm using symmerge

    template<typename RandomAccessIterator1, typename RandomAccessIterator2,
             typename Compare, typename Projection>
    auto inplace_merge(RandomAccessIterator1 first, RandomAccessIterator1 middle,
                       RandomAccessIterator1 last,
                       Compare compare, Projection projection,
                       difference_type_t<RandomAccessIterator1> len1,
                       difference_type_t<RandomAccessIterator1> len2,
                       RandomAccessIterator2 buff, std::ptrdiff_t buff_size,
                       std::random_access_iterator_tag)
        -> void
    {
        symmerge(first, 0, middle - first, last - first,
                 std::move(compare), std::move(projection),
                 len1, len2, buff, buff_size);
    }

    ////////////////////////////////////////////////////////////
    // Intermediate functions allocating the memory buffer if
    // one wasn't passed

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle,
                       ForwardIterator last, Compare compare, Projection projection,
                       std::forward_iterator_tag)
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

        auto n0 = std::distance(first, middle);
        auto n1 = std::distance(middle, last);

        auto buffer = temporary_buffer<rvalue_type_t<ForwardIterator>>((std::max)(n0, n1));
        recmerge(std::move(first), n0, std::move(middle), n1,
                 buffer.data(), buffer.size(),
                 std::move(compare), std::move(projection));
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<BidirectionalIterator> len1,
                       difference_type_t<BidirectionalIterator> len2,
                       std::bidirectional_iterator_tag)
        -> void
    {
        temporary_buffer<rvalue_type_t<BidirectionalIterator>> buffer((std::min)(len1, len2));

        using category = iterator_category_t<BidirectionalIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buffer.data(), buffer.size(),
                      category{});
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last, Compare compare, Projection projection,
                       std::bidirectional_iterator_tag)
        -> void
    {
        using category = iterator_category_t<BidirectionalIterator>;
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Shrink the problem size on the left side, makes the
        // size computation potentially cheaper
        while (first != middle && not comp(proj(*middle), proj(*first))) {
            ++first;
        }
        if (first == middle) return;

        auto len1 = std::distance(first, middle);
        auto len2 = std::distance(middle, last);

        temporary_buffer<rvalue_type_t<BidirectionalIterator>> buffer((std::min)(len1, len2));
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buffer.data(), buffer.size(),
                      category{});
    }

    ////////////////////////////////////////////////////////////
    // Generic inplace_merge interfaces

    // Unbuffered overload, defers the buffer allocation to a specific
    // function depending on the iterator category

    template<typename ForwardIterator, typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                       Compare compare, Projection projection)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      category{});
    }

    template<typename BidirectionalIterator, typename Compare, typename Projection>
    auto inplace_merge(BidirectionalIterator first, BidirectionalIterator middle,
                       BidirectionalIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<BidirectionalIterator> len1,
                       difference_type_t<BidirectionalIterator> len2)
        -> void
    {
        using category = iterator_category_t<BidirectionalIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, category{});
    }

    // Buffered overload, which also happens to take the length of the
    // subranges, allowing not to cross the whole range to compute them
    // from time to time

    template<typename ForwardIterator, typename RandomAccessIterator,
             typename Compare, typename Projection>
    auto inplace_merge(ForwardIterator first, ForwardIterator middle, ForwardIterator last,
                       Compare compare, Projection projection,
                       difference_type_t<ForwardIterator> len1,
                       difference_type_t<ForwardIterator> len2,
                       RandomAccessIterator buff, std::ptrdiff_t buff_size)
        -> void
    {
        using category = iterator_category_t<ForwardIterator>;
        inplace_merge(std::move(first), std::move(middle), std::move(last),
                      std::move(compare), std::move(projection),
                      len1, len2, buff, buff_size,
                      category{});
    }
}}

#endif // CPPSORT_DETAIL_INPLACE_MERGE_H_
