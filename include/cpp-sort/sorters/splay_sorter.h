/*
 * Copyright (c) 2022-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPLAY_SORTER_H_
#define CPPSORT_SORTERS_SPLAY_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include "../detail/iterator_traits.h"
#include "../detail/splaysort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct splay_sorter_impl
        {
            template<
                typename ForwardRange,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_v<Projection, ForwardRange, Compare>
                >
            >
            auto operator()(ForwardRange&& range,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<decltype(std::begin(range))>
                    >,
                    "splay_sorter requires at least forward iterators"
                );

                splaysort(std::begin(range), std::end(range),
                          utility::size(range),
                          std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >,
                    "splay_sorter requires at least forward iterators"
                );

                auto size = std::distance(first, last);
                splaysort(std::move(first), std::move(last), size,
                          std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    struct splay_sorter:
        sorter_facade<detail::splay_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr splay_sorter splay_sort{};
}

#endif // CPPSORT_SORTERS_SPLAY_SORTER_H_
