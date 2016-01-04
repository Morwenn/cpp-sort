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

namespace cppsort
{
namespace detail
{
    template<typename Compare>
    struct three_way_compare
    {
        public:

            constexpr three_way_compare(Compare compare):
                compare(compare)
            {}

            template<typename T, typename U>
            constexpr auto operator()(const T& lhs, const U& rhs) const
                -> int
            {
                return compare(lhs, rhs) ? -1 : compare(rhs, lhs);
            }

            constexpr auto base() const noexcept
                -> Compare
            {
                return compare;
            }

        private:

            Compare compare;
    };

    template<>
    struct three_way_compare<std::less<>>
    {
        constexpr three_way_compare(std::less<>) {}

        template<typename T, typename U>
        constexpr auto operator()(const T& lhs, const U& rhs) const
            -> int
        {
            std::less<> compare;
            return compare(lhs, rhs) ? -1 : compare(rhs, lhs);
        }

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
    struct three_way_compare<std::greater<>>
    {
        constexpr three_way_compare(std::greater<>) {}

        template<typename T, typename U>
        constexpr auto operator()(const T& lhs, const U& rhs) const
            -> int
        {
            std::greater<> compare;
            return compare(lhs, rhs) ? -1 : compare(rhs, lhs);
        }

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
