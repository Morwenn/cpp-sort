/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_UTILITY_FUNCTIONAL_H_
#define CPPSORT_UTILITY_FUNCTIONAL_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <concepts>
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

    // CRTP base class for all projection objects: it is modelled after
    // std::ranges::range_adaptor_closure and thus uses CRTP the same
    // reason, namely to avoid having function objects bigger than they
    // ought to (which happens when the same base class is inherited
    // from several times by the same class)
    template<typename Derived>
        requires std::is_object_v<Derived> &&
                 std::same_as<Derived, std::remove_cv_t<Derived>>
    struct projection_base {};

    namespace detail
    {
        template<typename T, typename U>
        struct projection_base_pipe_result:
            projection_base<projection_base_pipe_result<T, U>>,
            cppsort::detail::raw_check_is_transparent<T, U>
        {
            [[no_unique_address]] T lhs;
            [[no_unique_address]] U rhs;

            projection_base_pipe_result(T lhs, U rhs):
                lhs(std::move(lhs)),
                rhs(std::move(rhs))
            {}

            template<typename Self, typename Arg>
            constexpr auto operator()(this Self&& self, Arg&& value)
                noexcept(noexcept(self.rhs(self.lhs(std::forward<Arg>(value)))))
                -> decltype(self.rhs(self.lhs(std::forward<Arg>(value))))
            {
                return self.rhs(self.lhs(std::forward<Arg>(value)));
            }
        };
    }

    template<typename T, typename U>
        requires (
            std::derived_from<std::remove_reference_t<T>, projection_base<std::remove_cvref_t<T>>> ||
            std::derived_from<std::remove_reference_t<U>, projection_base<std::remove_cvref_t<U>>>
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
        requires std::derived_from<std::remove_reference_t<T>, projection_base<std::remove_cvref_t<T>>>
    constexpr auto operator|(T&& lhs, std::identity)
        noexcept(noexcept(std::forward<T>(lhs)))
        -> decltype(std::forward<T>(lhs))
    {
        return std::forward<T>(lhs);
    }

    template<typename U>
        requires std::derived_from<std::remove_reference_t<U>, projection_base<std::remove_cvref_t<U>>>
    constexpr auto operator|(std::identity, U&& rhs)
        noexcept(noexcept(std::forward<U>(rhs)))
        -> decltype(std::forward<U>(rhs))
    {
        return std::forward<U>(rhs);
    }

    ////////////////////////////////////////////////////////////
    // indirect

    struct indirect:
        projection_base<indirect>
    {
        template<typename T>
        static constexpr auto operator()(T&& indirect_value)
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
            projection_base<as_projection_fn<Function>>,
            cppsort::detail::raw_check_is_transparent<Function>
        {
            private:

                [[no_unique_address]] Function _func;

            public:

                as_projection_fn() = default;
                as_projection_fn(const as_projection_fn&) = default;
                as_projection_fn(as_projection_fn&&) = default;

                template<typename Func>
                    requires (not std::is_same_v<std::remove_cvref_t<Func>, as_projection_fn>)
                constexpr explicit as_projection_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename Self, typename T>
                constexpr auto operator()(this Self&& self, T&& arg)
                    noexcept(noexcept(as_function(std::forward<Self>(self)._func)(std::forward<T>(arg))))
                    -> decltype(as_function(std::forward<Self>(self)._func)(std::forward<T>(arg)))
                {
                    return as_function(std::forward<Self>(self)._func)(std::forward<T>(arg));
                }
        };

        template<typename Function>
        struct as_comparison_fn:
            cppsort::detail::raw_check_is_transparent<Function>
        {
            private:

                [[no_unique_address]] Function _func;

            public:

                as_comparison_fn() = default;
                as_comparison_fn(const as_comparison_fn&) = default;
                as_comparison_fn(as_comparison_fn&&) = default;

                template<typename Func>
                    requires (not std::is_same_v<std::remove_cvref_t<Func>, as_comparison_fn>)
                constexpr explicit as_comparison_fn(Func&& func):
                    _func(std::forward<Func>(func))
                {}

                template<typename Self, typename T, typename U>
                constexpr auto operator()(this Self&& self, T&& lhs, U&& rhs)
                    noexcept(noexcept(as_function(std::forward<Self>(self)._func)(
                        std::forward<T>(lhs), std::forward<U>(rhs))))
                    -> decltype(as_function(std::forward<Self>(self)._func)(
                        std::forward<T>(lhs), std::forward<U>(rhs)))
                {
                    return as_function(std::forward<Self>(self)._func)(
                        std::forward<T>(lhs),
                        std::forward<U>(rhs)
                    );
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
        projection_base<half>
    {
        template<typename T>
        static constexpr auto operator()(T&& value)
            -> decltype(std::forward<T>(value) / 2)
        {
            return std::forward<T>(value) / 2;
        }

        using is_transparent = void;
    };

    struct log:
        projection_base<log>
    {
        template<typename T>
        static constexpr auto operator()(T&& value)
            -> decltype(auto)
        {
            using std::log;
            return log(std::forward<T>(value));
        }

        using is_transparent = void;
    };

    struct sqrt:
        projection_base<sqrt>
    {
        template<typename T>
        static constexpr auto operator()(T&& value)
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
        static constexpr auto operator()(Args&&... args)
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
