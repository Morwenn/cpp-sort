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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/drop_merge_sort.h"
#include "../detail/type_traits.h"

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
                typename BidirectionalIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, BidirectionalIterator, Compare>
                >
            >
            auto operator()(BidirectionalIterator first, BidirectionalIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<BidirectionalIterator>
                    >::value,
                    "drop_merge_adapter requires at least bidirectional iterators"
                );

                drop_merge_sort(std::move(first), std::move(last),
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
