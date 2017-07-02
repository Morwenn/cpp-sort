/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#ifndef CPPSORT_UTILITY_FUNCTIONAL_H_
#define CPPSORT_UTILITY_FUNCTIONAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/branchless_traits.h>

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Identity (mostly useful for projections)

    struct identity
    {
        template<typename T>
        constexpr auto operator()(T&& value) const noexcept
            -> T&&
        {
            return std::forward<T>(value);
        }

        using is_transparent = void;
    };

    template<typename T>
    struct is_probably_branchless_projection<identity, T>:
        std::true_type
    {};

    ////////////////////////////////////////////////////////////
    // Transform overload in unary or binary function

    namespace detail
    {
        template<typename Function>
        struct as_projection_fn
        {
            private:

                Function _func;

            public:

                as_projection_fn() = delete;
                as_projection_fn(const as_projection_fn&) = default;
                as_projection_fn(as_projection_fn&&) = default;

                template<
                    typename Func,
                    typename = std::enable_if_t<
                        not std::is_same<std::decay_t<Func>, as_projection_fn>::value
                    >
                >
                constexpr explicit as_projection_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename T>
                constexpr auto operator()(T&& arg) &
                    noexcept(noexcept(_func(std::forward<T>(arg))))
                    -> decltype(_func(std::forward<T>(arg)))
                {
                    return _func(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) &&
                    noexcept(noexcept(std::move(_func)(std::forward<T>(arg))))
                    -> decltype(std::move(_func)(std::forward<T>(arg)))
                {
                    return std::move(_func)(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) const&
                    noexcept(noexcept(_func(std::forward<T>(arg))))
                    -> decltype(_func(std::forward<T>(arg)))
                {
                    return _func(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) const&&
                    noexcept(noexcept(std::move(_func)(std::forward<T>(arg))))
                    -> decltype(std::move(_func)(std::forward<T>(arg)))
                {
                    return std::move(_func)(std::forward<T>(arg));
                }
        };

        template<typename T>
        struct is_as_projection_fn:
            std::false_type
        {};

        template<typename T>
        struct is_as_projection_fn<as_projection_fn<T>>:
            std::true_type
        {};

        template<typename Function>
        struct as_comparison_fn
        {
            private:

                Function _func;

            public:

                as_comparison_fn() = delete;
                as_comparison_fn(const as_comparison_fn&) = default;
                as_comparison_fn(as_comparison_fn&&) = default;

                template<
                    typename Func,
                    typename = std::enable_if_t<
                        not std::is_same<std::decay_t<Func>, as_comparison_fn>::value
                    >
                >
                constexpr explicit as_comparison_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) &
                    noexcept(noexcept(_func(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(_func(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return _func(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) &&
                    noexcept(noexcept(std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) const&
                    noexcept(noexcept(_func(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(_func(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return _func(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) const&&
                    noexcept(noexcept(std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return std::move(_func)(std::forward<T>(lhs), std::forward<U>(rhs));
                }
        };

        template<typename T>
        struct is_as_comparison_fn:
            std::false_type
        {};

        template<typename T>
        struct is_as_comparison_fn<as_comparison_fn<T>>:
            std::true_type
        {};
    }

    template<typename Function>
    constexpr auto as_projection(Function&& func)
        -> std::enable_if_t<
            not detail::is_as_projection_fn<std::decay_t<Function>>::value,
            detail::as_projection_fn<std::decay_t<Function>>
        >
    {
        return detail::as_projection_fn<std::decay_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
    constexpr auto as_projection(Function&& func)
        -> std::enable_if_t<
            detail::is_as_projection_fn<std::decay_t<Function>>::value,
            decltype(std::forward<Function>(func))
        >
    {
        return std::forward<Function>(func);
    }

    template<typename Function>
    constexpr auto as_comparison(Function&& func)
        -> std::enable_if_t<
            not detail::is_as_comparison_fn<std::decay_t<Function>>::value,
            detail::as_comparison_fn<std::decay_t<Function>>
        >
    {
        return detail::as_comparison_fn<std::decay_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
    constexpr auto as_comparison(Function&& func)
        -> std::enable_if_t<
            detail::is_as_comparison_fn<std::decay_t<Function>>::value,
            decltype(std::forward<Function>(func))
        >
    {
        return std::forward<Function>(func);
    }

    ////////////////////////////////////////////////////////////
    // Math functions (mostly useful for buffer providers)

    struct half
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(std::forward<T>(value) / 2)
        {
            return std::forward<T>(value) / 2;
        }
    };

    struct log
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(auto)
        {
            using std::log;
            return log(std::forward<T>(value));
        }
    };

    struct sqrt
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(auto)
        {
            using std::sqrt;
            return sqrt(std::forward<T>(value));
        }
    };
}}

#endif // CPPSORT_UTILITY_FUNCTIONAL_H_
