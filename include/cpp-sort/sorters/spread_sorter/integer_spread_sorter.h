/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstdint>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/integer_sort.h"
#include "../../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct integer_spread_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_integral<projected_t<RandomAccessIterator, Projection>>::value && (
                        sizeof(projected_t<RandomAccessIterator, Projection>) <= sizeof(std::size_t) ||
                        sizeof(projected_t<RandomAccessIterator, Projection>) <= sizeof(std::uintmax_t)
                    ) &&
                    is_projection_iterator_v<Projection, RandomAccessIterator>
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "integer_spread_sorter requires at least random-access iterators"
                );

                spreadsort::integer_sort(std::move(first), std::move(last), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct integer_spread_sorter:
        sorter_facade<detail::integer_spread_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& integer_spread_sort
            = utility::static_const<integer_spread_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_
