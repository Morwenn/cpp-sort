/*
 * Copyright (c) 2017-2020 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/vergesort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename FallbackSorter>
        struct verge_adapter_impl:
            utility::adapter_storage<FallbackSorter>
        {
            verge_adapter_impl() = default;

            constexpr explicit verge_adapter_impl(FallbackSorter&& sorter):
                utility::adapter_storage<FallbackSorter>(std::move(sorter))
            {}

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
                    "verge_adapter requires at least random-access iterators"
                );

                vergesort(std::move(first), std::move(last), last - first,
                          std::move(compare), std::move(projection),
                          this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    template<typename FallbackSorter>
    struct verge_adapter:
        sorter_facade<detail::verge_adapter_impl<FallbackSorter>>
    {
        verge_adapter() = default;

        constexpr explicit verge_adapter(FallbackSorter sorter):
            sorter_facade<detail::verge_adapter_impl<FallbackSorter>>(std::move(sorter))
        {}
    };
}

#endif // CPPSORT_ADAPTERS_VERGE_ADAPTER_H_
