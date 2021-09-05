/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
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
#include "../detail/type_traits.h"

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
        auto case_insensitive_less(const T& lhs, const T& rhs, const std::locale& loc)
            -> bool
        {
            using char_type = remove_cvref_t<decltype(*std::begin(lhs))>;
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

        struct case_insensitive_less_fn;
        struct case_insensitive_less_locale_fn;
        namespace adl_barrier
        {
            template<typename T>
            struct refined_case_insensitive_less_fn;
            template<typename T>
            struct refined_case_insensitive_less_locale_fn;
        }

        template<typename T>
        using can_be_refined_for_t
            = decltype(*std::begin(std::declval<T&>()));

        template<typename T>
        constexpr bool can_be_refined_for
            = is_detected_v<can_be_refined_for_t, T>;

        struct case_insensitive_less_locale_fn
        {
            private:

                std::locale loc;

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

                template<
                    typename T,
                    typename = detail::enable_if_t<can_be_refined_for<T>>
                >
                auto refine() const
                    -> adl_barrier::refined_case_insensitive_less_locale_fn<T>
                {
                    return adl_barrier::refined_case_insensitive_less_locale_fn<T>(loc);
                }

                using is_transparent = void;
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

            template<
                typename T,
                typename = detail::enable_if_t<can_be_refined_for<T>>
            >
            auto refine() const
                -> adl_barrier::refined_case_insensitive_less_fn<T>
            {
                return adl_barrier::refined_case_insensitive_less_fn<T>();
            }

            inline auto operator()(const std::locale& loc) const
                -> case_insensitive_less_locale_fn
            {
                return case_insensitive_less_locale_fn(loc);
            }

            using is_transparent = void;
        };

        namespace adl_barrier
        {
            // Hide the generic case_insensitive_less from the enclosing namespace
            struct nope_type {};
            template<typename T>
            auto case_insensitive_less(const T& lhs, const T& rhs, const std::locale& loc)
                -> nope_type;
            template<typename T>
            auto case_insensitive_less(const T& lhs, const T& rhs)
                -> nope_type;

            // It makes is_invocable easier to work with
            struct caller
            {
                template<typename T>
                auto operator()(const T& lhs, const T& rhs, const std::locale& loc) const
                    -> decltype(case_insensitive_less(lhs, rhs, loc));

                template<typename T>
                auto operator()(const T& lhs, const T& rhs) const
                    -> decltype(case_insensitive_less(lhs, rhs));
            };

            template<typename T>
            struct refined_case_insensitive_less_locale_fn
            {
                private:

                    using char_type = remove_cvref_t<decltype(*std::begin(std::declval<T&>()))>;

                    std::locale loc;
                    const std::ctype<char_type>& ct;

                public:

                    explicit refined_case_insensitive_less_locale_fn(const std::locale& loc):
                        loc(loc),
                        ct(std::use_facet<std::ctype<char_type>>(loc))
                    {}

                    template<typename U=T>
                    auto operator()(const T& lhs, const T& rhs) const
                        -> detail::enable_if_t<
                            not is_invocable_r_v<nope_type, caller, U, U, std::locale>,
                            decltype(case_insensitive_less(lhs, rhs, loc))
                        >
                    {
                        return case_insensitive_less(lhs, rhs, loc);
                    }

                    template<typename U=T>
                    auto operator()(const T& lhs, const T& rhs) const
                        -> detail::enable_if_t<
                            is_invocable_r_v<nope_type, caller, U, U, std::locale>,
                            bool
                        >
                    {
                        return std::lexicographical_compare(std::begin(lhs), std::end(lhs),
                                                            std::begin(rhs), std::end(rhs),
                                                            char_less<char_type>(ct));
                    }
            };

            template<typename T>
            struct refined_case_insensitive_less_fn
            {
                private:

                    using char_type = remove_cvref_t<decltype(*std::begin(std::declval<T&>()))>;

                    std::locale loc;
                    const std::ctype<char_type>& ct;

                public:

                    refined_case_insensitive_less_fn():
                        loc(),
                        ct(std::use_facet<std::ctype<char_type>>(loc))
                    {}

                    template<typename U=T>
                    auto operator()(const T& lhs, const T& rhs) const
                        -> detail::enable_if_t<
                            negation<is_invocable_r<nope_type, caller, U, U>>::value,
                            decltype(case_insensitive_less(lhs, rhs))
                        >
                    {
                        return case_insensitive_less(lhs, rhs);
                    }

                    template<typename U=T>
                    auto operator()(const T& lhs, const T& rhs) const
                        -> detail::enable_if_t<
                            conjunction<
                                is_invocable_r<nope_type, caller, U, U>,
                                negation<is_invocable_r<nope_type, caller, U, U, std::locale>>
                            >::value,
                            decltype(case_insensitive_less(lhs, rhs, loc))
                        >
                    {
                        return case_insensitive_less(lhs, rhs, loc);
                    }

                    template<typename U=T>
                    auto operator()(const T& lhs, const T& rhs) const
                        -> detail::enable_if_t<
                            conjunction<
                                is_invocable_r<nope_type, caller, U, U>,
                                is_invocable_r<nope_type, caller, U, U, std::locale>
                            >::value,
                            bool
                        >
                    {
                        return std::lexicographical_compare(std::begin(lhs), std::end(lhs),
                                                            std::begin(rhs), std::end(rhs),
                                                            char_less<char_type>(ct));
                    }

                    auto operator()(const std::locale& loc) const
                        -> refined_case_insensitive_less_locale_fn<T>
                    {
                        return refined_case_insensitive_less_locale_fn<T>(loc);
                    }
            };
        }
    }

    using case_insensitive_less_t = detail::case_insensitive_less_fn;

    namespace
    {
        constexpr auto&& case_insensitive_less = utility::static_const<
            detail::case_insensitive_less_fn
        >::value;
    }
}

#endif // CPPSORT_COMPARATORS_CASE_INSENSITIVE_LESS_H_
