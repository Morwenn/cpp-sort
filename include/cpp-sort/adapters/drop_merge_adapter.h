/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_DROP_MERGE_ADAPTER_H_
#define CPPSORT_ADAPTERS_DROP_MERGE_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/drop_merge_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct drop_merge_adapter_impl:
            utility::adapter_storage<Sorter>
        {
            drop_merge_adapter_impl() = default;

            constexpr explicit drop_merge_adapter_impl(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {}

            template<
                mstd::bidirectional_iterator Iterator,
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
                auto last_it = mstd::next(first, std::move(last));
                drop_merge_sort(std::move(first), std::move(last_it),
                                std::move(compare), std::move(projection),
                                this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    template<typename Sorter>
    struct drop_merge_adapter:
        sorter_facade<detail::drop_merge_adapter_impl<Sorter>>
    {
        drop_merge_adapter() = default;

        constexpr explicit drop_merge_adapter(Sorter sorter):
            sorter_facade<detail::drop_merge_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };
}

#endif // CPPSORT_ADAPTERS_DROP_MERGE_ADAPTER_H_