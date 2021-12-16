/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/float_sort.h"
#include "../../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct float_spread_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<
                    std::numeric_limits<projected_t<RandomAccessIterator, Projection>>::is_iec559 && (
                        sizeof(projected_t<RandomAccessIterator, Projection>) == sizeof(std::uint32_t) ||
                        sizeof(projected_t<RandomAccessIterator, Projection>) == sizeof(std::uint64_t)
                    ) &&
                    is_projection_iterator_v<Projection, RandomAccessIterator>
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "float_spread_sorter requires at least random-access iterators"
                );

                spreadsort::float_sort(std::move(first), std::move(last), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct float_spread_sorter:
        sorter_facade<detail::float_spread_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& float_spread_sort
            = utility::static_const<float_spread_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
