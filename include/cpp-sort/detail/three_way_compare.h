/*
 * Copyright (c) 2016-2020 Morwenn
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

namespace cppsort
{
namespace detail
{
    template<typename Derived>
    struct three_way_compare_base
    {
        public:

            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                -> int
            {
                auto&& compare = derived().base();
                return compare(std::forward<T>(lhs), std::forward<U>(rhs)) ? -1 :
                       compare(std::forward<U>(rhs), std::forward<T>(lhs));
            }

            template<typename T, typename U>
            auto lt(T&& x, U&& y) const
                -> decltype(auto)
            {
                return derived().base()(std::forward<T>(x), std::forward<U>(y));
            }

            template<typename T, typename U>
            auto le(T&& x, U&& y) const
                -> decltype(auto)
            {
                return not derived().base()(std::forward<U>(y), std::forward<T>(x));
            }

            template<typename T, typename U>
            auto gt(T&& x, U&& y) const
                -> decltype(auto)
            {
                return derived().base()(std::forward<U>(y), std::forward<T>(x));
            }

            template<typename T, typename U>
            auto ge(T&& x, U&& y) const
                -> decltype(auto)
            {
                return not derived().base()(std::forward<T>(x), std::forward<U>(y));
            }

        private:

            auto derived() noexcept
                -> Derived&
            {
                return static_cast<Derived&>(*this);
            }

            auto derived() const noexcept
                -> const Derived&
            {
                return static_cast<const Derived&>(*this);
            }
    };

    template<
        typename Compare,
        bool = std::is_empty<Compare>::value && std::is_default_constructible<Compare>::value
    >
    struct three_way_compare:
        three_way_compare_base<three_way_compare<Compare>>
    {
        public:

            constexpr three_way_compare(Compare compare):
                compare(std::move(compare))
            {}

            constexpr auto base() const noexcept
                -> Compare
            {
                return compare;
            }

        private:

            Compare compare;
    };

    template<typename Compare>
    struct three_way_compare<Compare, true>:
        three_way_compare_base<three_way_compare<Compare>>
    {
        constexpr three_way_compare(Compare) {}

        using three_way_compare_base<three_way_compare<Compare>>::operator();

        constexpr auto base() const noexcept
            -> Compare
        {
            // If the comparator is empty, we don't need to store it
            return {};
        }
    };

    template<>
    struct three_way_compare<std::less<>, true>:
        three_way_compare_base<three_way_compare<std::less<>>>
    {
        constexpr three_way_compare(std::less<>) {}

        using three_way_compare_base<three_way_compare<std::less<>>>::operator();

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

        constexpr auto base() const noexcept
            -> std::less<>
        {
            return {};
        }
    };

#ifdef __cpp_lib_ranges
    template<>
    struct three_way_compare<std::ranges::less, true>:
        three_way_compare_base<three_way_compare<std::ranges::less>>
    {
        constexpr three_way_compare(std::ranges::less) {}

        using three_way_compare_base<three_way_compare<std::ranges::less>>::operator();

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

        constexpr auto base() const noexcept
            -> std::ranges::less
        {
            return {};
        }
    };
#endif

    template<>
    struct three_way_compare<std::greater<>, true>:
        three_way_compare_base<three_way_compare<std::greater<>>>
    {
        constexpr three_way_compare(std::greater<>) {}

        using three_way_compare_base<three_way_compare<std::greater<>>>::operator();

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

        constexpr auto base() const noexcept
            -> std::greater<>
        {
            return {};
        }
    };

#ifdef __cpp_lib_ranges
    template<>
    struct three_way_compare<std::ranges::greater, true>:
        three_way_compare_base<three_way_compare<std::ranges::greater>>
    {
        constexpr three_way_compare(std::ranges::greater) {}

        using three_way_compare_base<three_way_compare<std::ranges::greater>>::operator();

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

        constexpr auto base() const noexcept
            -> std::ranges::greater
        {
            return {};
        }
    };
#endif
}}

#endif // CPPSORT_DETAIL_THREE_WAY_COMPARE_H_
