/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_VERGE_SORTER_H_
#define CPPSORT_SORTERS_VERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/fwd.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/vergesort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter implementation

    namespace detail
    {
        template<bool Stable>
        struct verge_sorter_impl
        {
            template<
                mstd::bidirectional_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> void
            {
                auto first = mstd::begin(range);
                auto last = mstd::end(range);
                auto last_it = mstd::next(first, std::move(last));
                verge::sort<Stable>(std::move(first), std::move(last_it),
                                    mstd::distance(range),
                                    std::move(compare), std::move(projection));
            }

            template<
                mstd::bidirectional_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto last_it = mstd::next(first, std::move(last));
                auto dist = mstd::distance(first, last);
                verge::sort<Stable>(std::move(first), std::move(last_it), dist,
                                    std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::bool_constant<Stable>;
        };
    }

    ////////////////////////////////////////////////////////////
    // Sorters

    struct verge_sorter:
        sorter_facade<detail::verge_sorter_impl<false>>
    {};

    template<>
    struct stable_adapter<verge_sorter>:
        sorter_facade<detail::verge_sorter_impl<true>>
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(verge_sorter):
            sorter_facade<detail::verge_sorter_impl<true>>()
        {}
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr verge_sorter verge_sort{};
}

#endif // CPPSORT_SORTERS_VERGE_SORTER_H_
