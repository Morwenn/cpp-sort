/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_STD_SORTER_H_
#define CPPSORT_SORTERS_STD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/fwd.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct std_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename = detail::enable_if_t<not is_projection_iterator_v<
                    Compare, RandomAccessIterator
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "std_sorter requires at least random-access iterators"
                );

                std::sort(std::move(first), std::move(last),
                          utility::as_function(compare));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct std_sorter:
        sorter_facade<detail::std_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Stable sorter

    namespace detail
    {
        struct std_stable_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename = detail::enable_if_t<not is_projection_iterator_v<
                    Compare, RandomAccessIterator
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}) const
                -> void
            {
                static_assert(
                    std::is_base_of<
                        std::random_access_iterator_tag,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "stable_adapter<std_sorter> requires at least random-access iterators"
                );

                std::stable_sort(std::move(first), std::move(last),
                                 utility::as_function(compare));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    template<>
    struct stable_adapter<std_sorter>:
        sorter_facade<detail::std_stable_sorter_impl>
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(std_sorter) noexcept:
            sorter_facade<detail::std_stable_sorter_impl>()
        {}
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& std_sort
            = utility::static_const<std_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_STD_SORTER_H_
