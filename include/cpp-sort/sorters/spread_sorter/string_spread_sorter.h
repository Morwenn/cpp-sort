/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstdint>
#include <functional>
#include <iterator>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/string_sort.h"
#include "../../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct string_spread_sorter_impl
        {
            ////////////////////////////////////////////////////////////
            // Ascending string sort

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string>
                    || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string_view>
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                unsigned char unused = '\0';
                spreadsort::string_sort(std::move(first), std::move(last),
                                        std::move(projection), unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<(
                        std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring>
                        || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                std::uint16_t unused = 0;
                spreadsort::string_sort(std::move(first), std::move(last),
                                        std::move(projection), unused);
            }

            ////////////////////////////////////////////////////////////
            // Descending string sort

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string>
                    || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string_view>
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                unsigned char unused = '\0';
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> detail::enable_if_t<(
                        std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring>
                        || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::ranges::greater compare, Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string>
                    || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::string_view>
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                unsigned char unused = '\0';
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = std::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::ranges::greater compare, Projection projection={}) const
                -> detail::enable_if_t<(
                        std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring>
                        || std::is_same_v<projected_t<RandomAccessIterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                >
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "string_spread_sorter requires at least random-access iterators"
                );

                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct string_spread_sorter:
        sorter_facade<detail::string_spread_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr string_spread_sorter string_spread_sort{};
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
