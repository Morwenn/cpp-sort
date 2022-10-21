/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_MERGE_SORTER_H_
#define CPPSORT_SORTERS_MERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/size.h>
#include "../detail/merge_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct merge_sorter_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, Range, Compare>
                >
            >
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> void
            {
                merge_sort(mstd::begin(range), mstd::end(range),
                           utility::size(range),
                           std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Iterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto dist = std::distance(first, last);
                merge_sort(std::move(first), std::move(last), dist,
                           std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    struct merge_sorter:
        sorter_facade<detail::merge_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr merge_sorter merge_sort{};
}

#ifdef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_
#include "../detail/container_aware/merge_sort.h"
#endif

#define CPPSORT_SORTERS_MERGE_SORTER_DONE_

#endif // CPPSORT_SORTERS_MERGE_SORTER_H_
