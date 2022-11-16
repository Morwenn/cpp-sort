/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_FUNCTIONAL_H_
#define CPPSORT_UTILITY_FUNCTIONAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/raw_checkers.h"
#include "../detail/type_traits.h"

namespace cppsort::utility
{
    ////////////////////////////////////////////////////////////
    // Base type to allow piping projections

    // Literally just to check that other classes
    // actually inherit from it
    struct projection_base {};

    namespace detail
    {
        template<typename T, typename U>
        struct projection_base_pipe_result:
            projection_base,
            cppsort::detail::raw_check_is_transparent<T, U>
        {
            [[no_unique_address]] T lhs;
            [[no_unique_address]] U rhs;

            projection_base_pipe_result(T lhs, U rhs):
                lhs(std::move(lhs)),
                rhs(std::move(rhs))
            {}

            template<typename Arg>
            constexpr auto operator()(Arg&& value)
                noexcept(noexcept(rhs(lhs(std::forward<Arg>(value)))))
                -> decltype(rhs(lhs(std::forward<Arg>(value))))
            {
                return rhs(lhs(std::forward<Arg>(value)));
            }

            template<typename Arg>
            constexpr auto operator()(Arg&& value) const
                noexcept(noexcept(rhs(lhs(std::forward<Arg>(value)))))
                -> decltype(rhs(lhs(std::forward<Arg>(value))))
            {
                return rhs(lhs(std::forward<Arg>(value)));
            }
        };
    }

    template<typename T, typename U>
        requires (
            std::is_base_of_v<projection_base, std::remove_cvref_t<T>> ||
            std::is_base_of_v<projection_base, std::remove_cvref_t<U>>
        )
    constexpr auto operator|(T&& lhs, U&& rhs)
        -> decltype(auto)
    {
        using lhs_t = std::remove_cvref_t<decltype(as_function(std::forward<T>(lhs)))>;
        using rhs_t = std::remove_cvref_t<decltype(as_function(std::forward<U>(rhs)))>;
        return detail::projection_base_pipe_result<lhs_t, rhs_t>(
            as_function(std::forward<T>(lhs)),
            as_function(std::forward<U>(rhs))
        );
    }

    template<typename T>
        requires std::is_base_of_v<projection_base, std::remove_cvref_t<T>>
    constexpr auto operator|(T&& lhs, std::identity)
        noexcept(noexcept(std::forward<T>(lhs)))
        -> decltype(std::forward<T>(lhs))
    {
        return std::forward<T>(lhs);
    }

    template<typename U>
        requires std::is_base_of_v<projection_base, std::remove_cvref_t<U>>
    constexpr auto operator|(std::identity, U&& rhs)
        noexcept(noexcept(std::forward<U>(rhs)))
        -> decltype(std::forward<U>(rhs))
    {
        return std::forward<U>(rhs);
    }

    ////////////////////////////////////////////////////////////
    // indirect

    struct indirect:
        projection_base
    {
        template<typename T>
        constexpr auto operator()(T&& indirect_value)
            -> decltype(*std::forward<T>(indirect_value))
        {
            return *std::forward<T>(indirect_value);
        }

        template<typename T>
        constexpr auto operator()(T&& indirect_value) const
            -> decltype(*std::forward<T>(indirect_value))
        {
            return *std::forward<T>(indirect_value);
        }
    };

    ////////////////////////////////////////////////////////////
    // Transform overload in unary or binary function

    namespace detail
    {
        template<typename Function>
        struct as_projection_fn:
            projection_base,
            cppsort::detail::raw_check_is_transparent<Function>
        {
            private:

                Function _func;

            public:

                as_projection_fn() = default;
                as_projection_fn(const as_projection_fn&) = default;
                as_projection_fn(as_projection_fn&&) = default;

                template<typename Func>
                    requires (not std::is_same_v<std::remove_cvref_t<Func>, as_projection_fn>)
                constexpr explicit as_projection_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename T>
                constexpr auto operator()(T&& arg) &
                    noexcept(noexcept(as_function(_func)(std::forward<T>(arg))))
                    -> decltype(as_function(_func)(std::forward<T>(arg)))
                {
                    return as_function(_func)(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) &&
                    noexcept(noexcept(std::move(as_function(_func))(std::forward<T>(arg))))
                    -> decltype(std::move(as_function(_func))(std::forward<T>(arg)))
                {
                    return std::move(as_function(_func))(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) const&
                    noexcept(noexcept(as_function(_func)(std::forward<T>(arg))))
                    -> decltype(as_function(_func)(std::forward<T>(arg)))
                {
                    return as_function(_func)(std::forward<T>(arg));
                }

                template<typename T>
                constexpr auto operator()(T&& arg) const&&
                    noexcept(noexcept(std::move(as_function(_func))(std::forward<T>(arg))))
                    -> decltype(std::move(as_function(_func))(std::forward<T>(arg)))
                {
                    return std::move(as_function(_func))(std::forward<T>(arg));
                }
        };

        template<typename Function>
        struct as_comparison_fn:
            cppsort::detail::raw_check_is_transparent<Function>
        {
            private:

                Function _func;

            public:

                as_comparison_fn() = default;
                as_comparison_fn(const as_comparison_fn&) = default;
                as_comparison_fn(as_comparison_fn&&) = default;

                template<typename Func>
                    requires (not std::is_same_v<std::remove_cvref_t<Func>, as_comparison_fn>)
                constexpr explicit as_comparison_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) &
                    noexcept(noexcept(as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) &&
                    noexcept(noexcept(std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) const&
                    noexcept(noexcept(as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return as_function(_func)(std::forward<T>(lhs), std::forward<U>(rhs));
                }

                template<typename T, typename U>
                constexpr auto operator()(T&& lhs, U&& rhs) const&&
                    noexcept(noexcept(std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return std::move(as_function(_func))(std::forward<T>(lhs), std::forward<U>(rhs));
                }
        };
    }

    template<typename Function>
        requires (not cppsort::detail::is_specialization_of_v<
            std::remove_cvref_t<Function>,
            detail::as_projection_fn
        >)
    constexpr auto as_projection(Function&& func)
        -> detail::as_projection_fn<std::remove_cvref_t<Function>>
    {
        return detail::as_projection_fn<std::remove_cvref_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
        requires cppsort::detail::is_specialization_of_v<
            std::remove_cvref_t<Function>,
            detail::as_projection_fn
        >
    constexpr auto as_projection(Function&& func)
        -> decltype(std::forward<Function>(func))
    {
        return std::forward<Function>(func);
    }

    template<typename Function>
        requires (not cppsort::detail::is_specialization_of_v<
            std::remove_cvref_t<Function>,
            detail::as_comparison_fn
        >)
    constexpr auto as_comparison(Function&& func)
        -> detail::as_comparison_fn<std::remove_cvref_t<Function>>
    {
        return detail::as_comparison_fn<std::remove_cvref_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
        requires cppsort::detail::is_specialization_of_v<
            std::remove_cvref_t<Function>,
            detail::as_comparison_fn
        >
    constexpr auto as_comparison(Function&& func)
        -> decltype(std::forward<Function>(func))
    {
        return std::forward<Function>(func);
    }

    ////////////////////////////////////////////////////////////
    // Math functions (mostly useful for buffer providers)

    struct half:
        projection_base
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(std::forward<T>(value) / 2)
        {
            return std::forward<T>(value) / 2;
        }

        using is_transparent = void;
    };

    struct log:
        projection_base
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(auto)
        {
            using std::log;
            return log(std::forward<T>(value));
        }

        using is_transparent = void;
    };

    struct sqrt:
        projection_base
    {
        template<typename T>
        constexpr auto operator()(T&& value) const
            -> decltype(auto)
        {
            using std::sqrt;
            return sqrt(std::forward<T>(value));
        }

        using is_transparent = void;
    };

    ////////////////////////////////////////////////////////////
    // Function constant (micro-optimization)

    template<auto Function>
    struct function_constant
    {
        using value_type = decltype(Function);

        static constexpr value_type value = Function;

        template<typename... Args>
        constexpr auto operator()(Args&&... args) const
            noexcept(noexcept(as_function(Function)(std::forward<Args>(args)...)))
            -> decltype(as_function(Function)(std::forward<Args>(args)...))
        {
            return as_function(Function)(std::forward<Args>(args)...);
        }

        constexpr operator value_type() const noexcept
        {
            return Function;
        }
    };

    template<auto Function, typename T>
    struct is_probably_branchless_comparison<function_constant<Function>, T>:
        is_probably_branchless_comparison<
            typename function_constant<Function>::value_type, T
        >
    {};

    template<auto Function, typename T>
    struct is_probably_branchless_projection<function_constant<Function>, T>:
        is_probably_branchless_projection<
            typename function_constant<Function>::value_type, T
        >
    {};
}

#endif // CPPSORT_UTILITY_FUNCTIONAL_H_
