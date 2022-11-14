/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_COUNTING_SORTER_H_
#define CPPSORT_SORTERS_COUNTING_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/concepts.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include "../detail/counting_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct counting_sorter_impl
        {
            template<mstd::forward_iterator Iterator, mstd::sentinel_for<Iterator> Sentinel>
                requires mstd::integral<std::iter_value_t<Iterator>>
            constexpr auto operator()(Iterator first, Sentinel last) const
                -> void
            {
                counting_sort(std::move(first), std::move(last));
            }

            template<mstd::forward_iterator Iterator, mstd::sentinel_for<Iterator> Sentinel>
                requires mstd::integral<std::iter_value_t<Iterator>>
            constexpr auto operator()(Iterator first, Sentinel last, std::greater<>) const
                -> void
            {
                reverse_counting_sort(std::move(first), std::move(last));
            }

            template<mstd::forward_iterator Iterator, mstd::sentinel_for<Iterator> Sentinel>
                requires mstd::integral<std::iter_value_t<Iterator>>
            constexpr auto operator()(Iterator first, Sentinel last, std::ranges::greater) const
                -> void
            {
                reverse_counting_sort(std::move(first), std::move(last));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct counting_sorter:
        sorter_facade<detail::counting_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr counting_sorter counting_sort{};
}

#endif // CPPSORT_SORTERS_COUNTING_SORTER_H_
