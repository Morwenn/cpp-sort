/*
 * Copyright (c) 2016-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_THREE_WAY_COMPARE_H_
#define CPPSORT_DETAIL_THREE_WAY_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <string>
#include <type_traits>
#include <utility>

namespace cppsort::detail
{
    struct three_way_compare_base
    {
        template<typename Self, typename T, typename U>
        constexpr auto operator()(this const Self& self, T&& lhs, U&& rhs)
            -> int
        {
            auto&& compare = self.base();
            return compare(std::forward<T>(lhs), std::forward<U>(rhs)) ? -1 :
                    compare(std::forward<U>(rhs), std::forward<T>(lhs));
        }

        template<typename Self, typename T, typename U>
        auto lt(this const Self& self, T&& x, U&& y)
            -> decltype(auto)
        {
            return self.base()(std::forward<T>(x), std::forward<U>(y));
        }

        template<typename Self, typename T, typename U>
        auto le(this const Self& self, T&& x, U&& y)
            -> decltype(auto)
        {
            return not self.base()(std::forward<U>(y), std::forward<T>(x));
        }

        template<typename Self, typename T, typename U>
        auto gt(this const Self& self, T&& x, U&& y)
            -> decltype(auto)
        {
            return self.base()(std::forward<U>(y), std::forward<T>(x));
        }

        template<typename Self, typename T, typename U>
        auto ge(this const Self& self, T&& x, U&& y)
            -> decltype(auto)
        {
            return not self.base()(std::forward<T>(x), std::forward<U>(y));
        }
    };

    template<
        typename Compare,
        bool = std::is_empty_v<Compare> && std::is_default_constructible_v<Compare>
    >
    struct three_way_compare:
        three_way_compare_base
    {
        public:

            constexpr explicit three_way_compare(Compare compare):
                compare(std::move(compare))
            {}

            [[nodiscard]]
            constexpr auto base() const noexcept
                -> Compare
            {
                return compare;
            }

        private:

            [[no_unique_address]] Compare compare;
    };

    template<typename Compare>
    struct three_way_compare<Compare, true>:
        three_way_compare_base
    {
        constexpr explicit three_way_compare(Compare) {}

        using three_way_compare_base::operator();

        [[nodiscard]]
        constexpr auto base() const noexcept
            -> Compare
        {
            // If the comparator is empty, we don't need to store it
            return {};
        }
    };

    template<>
    struct three_way_compare<std::less<>, true>:
        three_way_compare_base
    {
        constexpr explicit three_way_compare(std::less<>) {}

        using three_way_compare_base::operator();

        template<
            typename CharT,
            typename Traits1, typename Alloc1,
            typename Traits2, typename Alloc2
        >
        auto operator()(const std::basic_string<CharT, Traits1, Alloc1>& lhs,
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs) const
            -> int
        {
            return lhs.compare(0, lhs.size(), rhs.data(), rhs.size());
        }

        [[nodiscard]]
        constexpr auto base() const noexcept
            -> std::less<>
        {
            return {};
        }
    };

    template<>
    struct three_way_compare<std::ranges::less, true>:
        three_way_compare_base
    {
        constexpr explicit three_way_compare(std::ranges::less) {}

        using three_way_compare_base::operator();

        template<
            typename CharT,
            typename Traits1, typename Alloc1,
            typename Traits2, typename Alloc2
        >
        auto operator()(const std::basic_string<CharT, Traits1, Alloc1>& lhs,
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs) const
            -> int
        {
            return lhs.compare(0, lhs.size(), rhs.data(), rhs.size());
        }

        [[nodiscard]]
        constexpr auto base() const noexcept
            -> std::ranges::less
        {
            return {};
        }
    };

    template<>
    struct three_way_compare<std::greater<>, true>:
        three_way_compare_base
    {
        constexpr explicit three_way_compare(std::greater<>) {}

        using three_way_compare_base::operator();

        template<
            typename CharT,
            typename Traits1, typename Alloc1,
            typename Traits2, typename Alloc2
        >
        auto operator()(const std::basic_string<CharT, Traits1, Alloc1>& lhs,
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs) const
            -> int
        {
            int res = lhs.compare(0, lhs.size(), rhs.data(), rhs.size());
            return (res < 0) ? 1 : -res;
        }

        [[nodiscard]]
        constexpr auto base() const noexcept
            -> std::greater<>
        {
            return {};
        }
    };

    template<>
    struct three_way_compare<std::ranges::greater, true>:
        three_way_compare_base
    {
        constexpr explicit three_way_compare(std::ranges::greater) {}

        using three_way_compare_base::operator();

        template<
            typename CharT,
            typename Traits1, typename Alloc1,
            typename Traits2, typename Alloc2
        >
        auto operator()(const std::basic_string<CharT, Traits1, Alloc1>& lhs,
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs) const
            -> int
        {
            int res = lhs.compare(0, lhs.size(), rhs.data(), rhs.size());
            return (res < 0) ? 1 : -res;
        }

        [[nodiscard]]
        constexpr auto base() const noexcept
            -> std::ranges::greater
        {
            return {};
        }
    };
}

#endif // CPPSORT_DETAIL_THREE_WAY_COMPARE_H_
