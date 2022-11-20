/*
 * Copyright (c) 2015-2022 Morwenn
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
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct std_sorter_impl
        {
            template<
                std::random_access_iterator Iterator,
                std::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>
            >
                requires (not is_projection_iterator_v<Compare, Iterator>)
            auto operator()(Iterator first, Sentinel last, Compare compare={}) const
                -> Iterator
            {
                auto last_it = mstd::next(first, std::move(last));
                std::sort(std::move(first), last_it, utility::as_function(compare));
                return last_it;
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
                std::random_access_iterator Iterator,
                typename Compare = std::less<>
            >
                requires (not is_projection_iterator_v<Compare, Iterator>)
            auto operator()(Iterator first, Iterator last, Compare compare={}) const
                -> void
            {
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

    inline constexpr std_sorter std_sort{};
}

#endif // CPPSORT_SORTERS_STD_SORTER_H_
