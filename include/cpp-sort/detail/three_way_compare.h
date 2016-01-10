/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CPPSORT_DETAIL_THREE_WAY_COMPARE_H_
#define CPPSORT_DETAIL_THREE_WAY_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <string>
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
                auto&& compare = derived().base();
                return compare(std::forward<T>(x), std::forward<U>(y))
                    || not compare(std::forward<U>(y), std::forward<T>(x));
            }

            template<typename T, typename U>
            auto gt(T&& x, U&& y) const
                -> decltype(auto)
            {
                auto&& compare = derived().base();
                return not compare(std::forward<T>(x), std::forward<U>(y))
                    && compare(std::forward<U>(y), std::forward<T>(x));
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

    template<typename Compare>
    struct three_way_compare:
        three_way_compare_base<three_way_compare<Compare>>
    {
        public:

            constexpr three_way_compare(Compare compare):
                compare(compare)
            {}

            constexpr auto base() const noexcept
                -> Compare
            {
                return compare;
            }

        private:

            Compare compare;
    };

    template<>
    struct three_way_compare<std::less<>>:
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
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs)
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

    template<>
    struct three_way_compare<std::greater<>>:
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
                        const std::basic_string<CharT, Traits2, Alloc2>& rhs)
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
}}

#endif // CPPSORT_DETAIL_THREE_WAY_COMPARE_H_
