/*
 * Copyright (c) 2022-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_SPLIT_ADAPTER_H_
#define CPPSORT_ADAPTERS_SPLIT_ADAPTER_H_

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
#include "../detail/split_sort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct split_adapter_impl:
            utility::adapter_storage<Sorter>
        {
            split_adapter_impl() = default;

            constexpr explicit split_adapter_impl(Sorter&& sorter):
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
                    "split_adapter requires a stronger iterator category"
                );

                split_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection),
                           this->get());
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = typename bidir_at_best_tag<Sorter>::type;
            using is_always_stable = std::false_type;
        };
    }

    template<typename Sorter>
    struct split_adapter:
        sorter_facade<detail::split_adapter_impl<Sorter>>
    {
        split_adapter() = default;

        constexpr explicit split_adapter(Sorter sorter):
            sorter_facade<detail::split_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };
}

#endif // CPPSORT_ADAPTERS_SPLIT_ADAPTER_H_
