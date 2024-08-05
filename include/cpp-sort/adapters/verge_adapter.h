/*
 * Copyright (c) 2017-2024 Morwenn
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
#include <cpp-sort/mstd/ranges.h>
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
        template<typename Sorter, bool Stable>
        struct verge_adapter_impl:
            utility::adapter_storage<Sorter>
        {
            verge_adapter_impl() = default;

            constexpr explicit verge_adapter_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                typename Self,
                mstd::bidirectional_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_v<Projection, Range, Compare>
            auto operator()(this Self&& self, Range&& range, Compare compare={}, Projection projection={})
                -> mstd::iterator_t<Range>
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<mstd::iterator_t<Range>>
                    >,
                    "split_adapter requires a stronger iterator category"
                );

                auto first = mstd::begin(range);
                auto last = mstd::end(range);
                auto last_it = mstd::next(first, std::move(last));
                verge::sort<Stable>(std::move(first), last_it,
                                    mstd::distance(range),
                                    std::move(compare), std::move(projection),
                                    std::forward<Self>(self).get());
                return last_it;
            }

            template<
                typename Self,
                mstd::bidirectional_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(this Self&& self, Iterator first, Sentinel last,
                            Compare compare={}, Projection projection={})
                -> Iterator
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<Iterator>
                    >,
                    "verge_adapter requires a stronger iterator category"
                );

                auto dist = mstd::distance(first, last);
                auto last_it = mstd::next(first, std::move(last));
                verge::sort<Stable>(std::move(first), last_it, dist,
                                    std::move(compare), std::move(projection),
                                    std::forward<Self>(self).get());
                return last_it;
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = cppsort::iterator_category<
                Sorter,
                std::bidirectional_iterator_tag
            >;
            using is_always_stable = std::bool_constant<Stable>;
        };
    }

    template<typename Sorter>
    struct verge_adapter:
        sorter_facade<detail::verge_adapter_impl<Sorter, false>>
    {
        verge_adapter() = default;

        constexpr explicit verge_adapter(Sorter sorter):
            sorter_facade<detail::verge_adapter_impl<Sorter, false>>(std::move(sorter))
        {}
    };

    template<typename Sorter>
    struct stable_adapter<verge_adapter<Sorter>>:
        sorter_facade<detail::verge_adapter_impl<Sorter, true>>
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(verge_adapter<Sorter> sorter):
            sorter_facade<detail::verge_adapter_impl<Sorter, true>>(std::move(sorter).get())
        {}
    };
}

#endif // CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
