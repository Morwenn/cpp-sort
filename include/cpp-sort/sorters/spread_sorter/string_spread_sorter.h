/*
 * Copyright (c) 2015-2021 Morwenn
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
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../../detail/config.h"
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/string_sort.h"
#include "../../detail/type_traits.h"

#if __cplusplus > 201402L && __has_include(<string_view>)
#   include <string_view>
#endif

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
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_same<projected_t<RandomAccessIterator, Projection>, std::string>::value
#if __cplusplus > 201402L && __has_include(<string_view>)
                    || std::is_same<projected_t<RandomAccessIterator, Projection>, std::string_view>::value
#endif
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "string_spread_sorter requires at least random-access iterators"
                );

                unsigned char unused = '\0';
                spreadsort::string_sort(std::move(first), std::move(last),
                                        std::move(projection), unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> detail::enable_if_t<(
                        std::is_same<projected_t<RandomAccessIterator, Projection>, std::wstring>::value
#if __cplusplus > 201402L && __has_include(<string_view>)
                        || std::is_same<projected_t<RandomAccessIterator, Projection>, std::wstring_view>::value
#endif
                    ) && (sizeof(wchar_t) == 2)
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
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
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> detail::enable_if_t<
                    std::is_same<projected_t<RandomAccessIterator, Projection>, std::string>::value
#if __cplusplus > 201402L && __has_include(<string_view>)
                    || std::is_same<projected_t<RandomAccessIterator, Projection>, std::string_view>::value
#endif
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "string_spread_sorter requires at least random-access iterators"
                );

                unsigned char unused = '\0';
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            std::greater<> compare, Projection projection={}) const
                -> detail::enable_if_t<(
                        std::is_same<projected_t<RandomAccessIterator, Projection>, std::wstring>::value
#if __cplusplus > 201402L && __has_include(<string_view>)
                        || std::is_same<projected_t<RandomAccessIterator, Projection>, std::wstring_view>::value
#endif
                    ) && (sizeof(wchar_t) == 2)
                >
            {
                static_assert(
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "string_spread_sorter requires at least random-access iterators"
                );

                std::uint16_t unused = 0;
                spreadsort::reverse_string_sort(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

#ifdef __cpp_lib_ranges
            template<
                typename RandomAccessIterator,
                typename Projection = utility::identity
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
                typename Projection = utility::identity
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
#endif

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

    namespace
    {
        constexpr auto&& string_spread_sort
            = utility::static_const<string_spread_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_STRING_SPREAD_SORTER_H_
