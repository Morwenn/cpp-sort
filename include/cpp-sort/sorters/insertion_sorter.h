/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_INSERTION_SORTER_H_
#define CPPSORT_SORTERS_INSERTION_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/insertion_sort.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct insertion_sorter_impl
        {
            template<
                typename BidirectionalIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, BidirectionalIterator, Compare>
                >
            >
            constexpr auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                                      Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<BidirectionalIterator>
                    >,
                    "insertion_sorter requires at least bidirectional iterators"
                );

                insertion_sort(std::move(first), std::move(last),
                               std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    struct insertion_sorter:
        sorter_facade<detail::insertion_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr insertion_sorter insertion_sort{};
}

#ifdef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_
#include "../detail/container_aware/insertion_sort.h"
#endif

#define CPPSORT_SORTERS_INSERTION_SORTER_DONE_

#endif // CPPSORT_SORTERS_INSERTION_SORTER_H_
