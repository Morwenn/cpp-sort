/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SLAB_SORTER_H_
#define CPPSORT_SORTERS_SLAB_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/size.h>
#include "../detail/iterator_traits.h"
#include "../detail/slabsort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct slab_sorter_impl
        {
            template<
                typename BidirectionalIterable,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, BidirectionalIterable, Compare>
                >
            >
            auto operator()(BidirectionalIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<decltype(std::begin(iterable))>
                    >,
                    "slab_sorter requires at least bidirectional iterators"
                );

                slabsort(std::begin(iterable), std::end(iterable),
                         utility::size(iterable),
                         std::move(compare), std::move(projection));
            }

            template<
                mstd::bidirectional_iterator Iterator,
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
                auto size = std::distance(first, last);
                slabsort(std::move(first), std::move(last), size,
                         std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct slab_sorter:
        sorter_facade<detail::slab_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr slab_sorter slab_sort{};
}

#endif // CPPSORT_SORTERS_SLAB_SORTER_H_
