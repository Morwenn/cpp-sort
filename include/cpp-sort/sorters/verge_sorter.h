/*
 * Copyright (c) 2015-2021 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"
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
                typename BidirectionalIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_v<Projection, BidirectionalIterable, Compare>
                >
            >
            auto operator()(BidirectionalIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<decltype(std::begin(iterable))>
                    >::value,
                    "verge_sorter requires at least bidirectional iterators"
                );

                verge::sort<Stable>(std::begin(iterable), std::end(iterable),
                                    utility::size(iterable),
                                    std::move(compare), std::move(projection));
            }

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
                    "verge_sorter requires at least bidirectional iterators"
                );

                auto size = std::distance(first, last);
                verge::sort<Stable>(std::move(first), std::move(last), size,
                                    std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::bidirectional_iterator_tag;
            using is_always_stable = std::integral_constant<bool, Stable>;
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

    namespace
    {
        constexpr auto&& verge_sort
            = utility::static_const<verge_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_VERGE_SORTER_H_
