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
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include "../../detail/iterator_traits.h"
#include "../../detail/spreadsort/string_sort.h"

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
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    std::is_same_v<projected_t<Iterator, Projection>, std::string> ||
                    std::is_same_v<projected_t<Iterator, Projection>, std::string_view>
                )
            auto operator()(Iterator first, Sentinel last, Projection projection={}) const
                -> void
            {
                unsigned char unused = '\0';
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::string_sort(std::move(first), std::move(last_it),
                                        std::move(projection), unused);
            }

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    (
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring> ||
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                )
            auto operator()(Iterator first, Sentinel last, Projection projection={}) const
                -> void
            {
                std::uint16_t unused = 0;
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::string_sort(std::move(first), std::move(last_it),
                                        std::move(projection), unused);
            }

            ////////////////////////////////////////////////////////////
            // Descending string sort

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    std::is_same_v<projected_t<Iterator, Projection>, std::string> ||
                    std::is_same_v<projected_t<Iterator, Projection>, std::string_view>
                )
            auto operator()(Iterator first, Sentinel last,
                            std::greater<> compare, Projection projection={}) const
                -> void
            {
                unsigned char unused = '\0';
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::reverse_string_sort(std::move(first), std::move(last_it),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    (
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring> ||
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                )
            auto operator()(Iterator first, Sentinel last,
                            std::greater<> compare, Projection projection={}) const
                -> void
            {
                std::uint16_t unused = 0;
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::reverse_string_sort(std::move(first), std::move(last_it),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    std::is_same_v<projected_t<Iterator, Projection>, std::string> ||
                    std::is_same_v<projected_t<Iterator, Projection>, std::string_view>
                )
            auto operator()(Iterator first, Sentinel last,
                            std::ranges::greater compare, Projection projection={}) const
                -> void
            {
                unsigned char unused = '\0';
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::reverse_string_sort(std::move(first), std::move(last_it),
                                                std::move(compare), std::move(projection),
                                                unused);
            }

            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Projection = std::identity
            >
                requires (
                    (
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring> ||
                        std::is_same_v<projected_t<Iterator, Projection>, std::wstring_view>
                    ) && (sizeof(wchar_t) == 2)
                )
            auto operator()(Iterator first, Sentinel last,
                            std::ranges::greater compare, Projection projection={}) const
                -> void
            {
                std::uint16_t unused = 0;
                auto last_it = mstd::next(first, std::move(last));
                spreadsort::reverse_string_sort(std::move(first), std::move(last_it),
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
