/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
#define CPPSORT_ADAPTERS_VERGE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/vergesort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename FallbackSorter, bool Stable>
        struct verge_adapter_impl:
            utility::adapter_storage<FallbackSorter>
        {
            verge_adapter_impl() = default;

            constexpr explicit verge_adapter_impl(FallbackSorter&& sorter):
                utility::adapter_storage<FallbackSorter>(std::move(sorter))
            {}

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            auto operator()(Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto dist = mstd::distance(first, last);
                auto last_it = mstd::next(first, std::move(last));
                verge::sort<Stable>(std::move(first), std::move(last_it), dist,
                                    std::move(compare), std::move(projection),
                                    this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::bool_constant<Stable>;
        };
    }

    template<typename FallbackSorter>
    struct verge_adapter:
        sorter_facade<detail::verge_adapter_impl<FallbackSorter, false>>
    {
        verge_adapter() = default;

        constexpr explicit verge_adapter(FallbackSorter sorter):
            sorter_facade<detail::verge_adapter_impl<FallbackSorter, false>>(std::move(sorter))
        {}
    };

    template<typename FallbackSorter>
    struct stable_adapter<verge_adapter<FallbackSorter>>:
        sorter_facade<detail::verge_adapter_impl<FallbackSorter, true>>
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(verge_adapter<FallbackSorter> sorter):
            sorter_facade<detail::verge_adapter_impl<FallbackSorter, true>>(std::move(sorter).get())
        {}
    };
}

#endif // CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
