/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>
#include <functional>
#include <iterator>
#include <limits>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/float_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct float_spread_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                typename Projection = std::identity
            >
            auto operator()(Iterator first, Iterator last, Projection projection={}) const
                -> mstd::enable_if_t<
                    std::numeric_limits<projected_t<Iterator, Projection>>::is_iec559 && (
                        sizeof(projected_t<Iterator, Projection>) == sizeof(std::uint32_t) ||
                        sizeof(projected_t<Iterator, Projection>) == sizeof(std::uint64_t)
                    ) &&
                    is_projection_iterator_v<Projection, Iterator>
                >
            {
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

    inline constexpr float_spread_sorter float_spread_sort{};
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_FLOAT_SPREAD_SORTER_H_
