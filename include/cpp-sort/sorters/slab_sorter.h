/*
 * Copyright (c) 2021 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
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
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                // TODO: make it work for bidirectional iterators
                static_assert(
                    std::is_base_of<
                        std::random_access_iterator_tag,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "slab_sorter requires at least random-access iterators"
                );

                slabsort(std::move(first), std::move(last),
                         std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct slab_sorter:
        sorter_facade<detail::slab_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& slab_sort
            = utility::static_const<slab_sorter>::value;
    }









    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct slab_sorter_impl2
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::random_access_iterator_tag,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "slab_sorter2 requires at least random-access iterators"
                );

                slabsort2(std::move(first), std::move(last),
                         std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct slab_sorter2:
        sorter_facade<detail::slab_sorter_impl2>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& slab_sort2
            = utility::static_const<slab_sorter2>::value;
    }
}

#endif // CPPSORT_SORTERS_SLAB_SORTER_H_
