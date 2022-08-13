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
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/raw_checkers.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace utility
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
            T lhs;
            U rhs;

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

    template<
        typename T,
        typename U,
        typename = cppsort::detail::enable_if_t<
            std::is_base_of<projection_base, cppsort::detail::remove_cvref_t<T>>::value ||
            std::is_base_of<projection_base, cppsort::detail::remove_cvref_t<U>>::value
        >
    >
    constexpr auto operator|(T&& lhs, U&& rhs)
        -> decltype(auto)
    {
        using lhs_t = cppsort::detail::remove_cvref_t<decltype(as_function(std::forward<T>(lhs)))>;
        using rhs_t = cppsort::detail::remove_cvref_t<decltype(as_function(std::forward<U>(rhs)))>;
        return detail::projection_base_pipe_result<lhs_t, rhs_t>(
            as_function(std::forward<T>(lhs)),
            as_function(std::forward<U>(rhs))
        );
    }

    ////////////////////////////////////////////////////////////
    // Identity (mostly useful for projections)

    struct identity:
        projection_base
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

                template<
                    typename Func,
                    typename = cppsort::detail::enable_if_t<
                        not std::is_same<cppsort::detail::remove_cvref_t<Func>, as_projection_fn>::value
                    >
                >
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

                template<
                    typename Func,
                    typename = cppsort::detail::enable_if_t<
                        not std::is_same<cppsort::detail::remove_cvref_t<Func>, as_comparison_fn>::value
                    >
                >
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
    constexpr auto as_projection(Function&& func)
        -> cppsort::detail::enable_if_t<
            not cppsort::detail::is_specialization_of_v<cppsort::detail::remove_cvref_t<Function>, detail::as_projection_fn>,
            detail::as_projection_fn<cppsort::detail::remove_cvref_t<Function>>
        >
    {
        return detail::as_projection_fn<cppsort::detail::remove_cvref_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
    constexpr auto as_projection(Function&& func)
        -> cppsort::detail::enable_if_t<
            cppsort::detail::is_specialization_of_v<cppsort::detail::remove_cvref_t<Function>, detail::as_projection_fn>,
            decltype(std::forward<Function>(func))
        >
    {
        return std::forward<Function>(func);
    }

    template<typename Function>
    constexpr auto as_comparison(Function&& func)
        -> cppsort::detail::enable_if_t<
            not cppsort::detail::is_specialization_of_v<cppsort::detail::remove_cvref_t<Function>, detail::as_comparison_fn>,
            detail::as_comparison_fn<cppsort::detail::remove_cvref_t<Function>>
        >
    {
        return detail::as_comparison_fn<cppsort::detail::remove_cvref_t<Function>>(std::forward<Function>(func));
    }

    template<typename Function>
    constexpr auto as_comparison(Function&& func)
        -> cppsort::detail::enable_if_t<
            cppsort::detail::is_specialization_of_v<cppsort::detail::remove_cvref_t<Function>, detail::as_comparison_fn>,
            decltype(std::forward<Function>(func))
        >
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

#ifdef __cpp_nontype_template_parameter_auto
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
#endif
}}

#endif // CPPSORT_UTILITY_FUNCTIONAL_H_
