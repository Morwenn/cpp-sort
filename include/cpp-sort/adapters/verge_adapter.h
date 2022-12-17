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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/vergesort.h"
#include "../detail/type_traits.h"

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
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "verge_adapter requires at least random-access iterators"
                );

                verge::sort<Stable>(std::move(first), std::move(last), last - first,
                                    std::move(compare), std::move(projection),
                                    this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::integral_constant<bool, Stable>;
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
