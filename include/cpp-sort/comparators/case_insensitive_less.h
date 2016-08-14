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
#ifndef CPPSORT_COMPARATORS_CASE_INSENSITIVE_LESS_H_
#define CPPSORT_COMPARATORS_CASE_INSENSITIVE_LESS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <locale>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/static_const.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Case insensitive comparison for char sequences

        template<typename CharT>
        struct char_less
        {
            const std::ctype<CharT>& ct;

            char_less(const std::ctype<CharT>& ct):
                ct(ct)
            {}

            auto operator()(CharT lhs, CharT rhs) const
                -> bool
            {
              return ct.tolower(lhs) < ct.tolower(rhs);
            }
        };

        template<typename T>
        auto case_insensitive_less(const T& lhs, const T& rhs, const std::locale loc)
            -> bool
        {
            using char_type = std::decay_t<decltype(*std::begin(lhs))>;
            const auto& ct = std::use_facet<std::ctype<char_type>>(loc);

            return std::lexicographical_compare(std::begin(lhs), std::end(lhs),
                                                std::begin(rhs), std::end(rhs),
                                                char_less<char_type>(ct));
        }

        template<typename T>
        auto case_insensitive_less(const T& lhs, const T& rhs)
            -> bool
        {
            std::locale loc;
            return case_insensitive_less(lhs, rhs, loc);
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        class case_insensitive_less_locale_fn
        {
            private:

                const std::locale& loc;

            public:

                explicit case_insensitive_less_locale_fn(const std::locale& loc):
                    loc(loc)
                {}

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) const
                    noexcept(noexcept(case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs), loc)))
                    -> decltype(case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs), loc))
                {
                    return case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs), loc);
                }
        };

        struct case_insensitive_less_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return case_insensitive_less(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            inline auto operator()(const std::locale& loc) const
                -> case_insensitive_less_locale_fn
            {
                return case_insensitive_less_locale_fn(loc);
            }
        };
    }

    namespace
    {
        constexpr auto&& case_insensitive_less = utility::static_const<
            detail::case_insensitive_less_fn
        >::value;
    }
}

#endif // CPPSORT_COMPARATORS_CASE_INSENSITIVE_LESS_H_
