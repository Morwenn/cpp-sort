/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SKA_SORTER_H_
#define CPPSORT_SORTERS_SKA_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/iterator_traits.h"
#include "../detail/ska_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct ska_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    is_projection_iterator_v<Projection, Iterator> &&
                    detail::is_ska_sortable_v<projected_t<Iterator, Projection>>
                )
            auto operator()(Iterator first, Sentinel last, Projection projection={}) const
                -> void
            {
                auto last_it = mstd::next(first, std::move(last));
                ska_sort(std::move(first), std::move(last_it), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct ska_sorter:
        sorter_facade<detail::ska_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr ska_sorter ska_sort{};
}

#endif // CPPSORT_SORTERS_SKA_SORTER_H_
