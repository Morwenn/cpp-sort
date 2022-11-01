/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <cstdint>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/integer_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct integer_spread_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
            auto operator()(Iterator first, Sentinel last, Projection projection={}) const
                -> mstd::enable_if_t<
                    std::is_integral_v<projected_t<Iterator, Projection>> && (
                        sizeof(projected_t<Iterator, Projection>) <= sizeof(std::size_t) ||
                        sizeof(projected_t<Iterator, Projection>) <= sizeof(std::uintmax_t)
                    ) &&
                    is_projection_iterator_v<Projection, Iterator>
                >
            {
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::integer_sort(std::move(first), std::move(last_it), std::move(projection));
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

    inline constexpr integer_spread_sorter integer_spread_sort{};
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_INTEGER_SPREAD_SORTER_H_
