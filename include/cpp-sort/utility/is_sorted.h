/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_CHECK_IS_SORTED_H_
#define CPPSORT_UTILITY_CHECK_IS_SORTED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/is_sorted_until.h"
#include "../detail/type_traits.h"

namespace cppsort::utility
{
    namespace detail
    {
        struct is_sorted_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            constexpr auto operator()(ForwardIterator first, ForwardIterator last,
                                      Compare compare={}, Projection projection={}) const
                -> bool
            {
                return cppsort::detail::is_sorted(
                    first, last,
                    std::move(compare), std::move(projection)
                );
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
        };
    }

    struct is_sorted_t:
        sorter_facade<detail::is_sorted_impl>
    {};

    inline constexpr is_sorted_t is_sorted{};
}

#endif // CPPSORT_UTILITY_CHECK_IS_SORTED_H_
