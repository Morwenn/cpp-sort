/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_NOT_FN_H_
#define CPPSORT_COMPARATORS_NOT_FN_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>

namespace cppsort
{
    template<typename F>
    struct flip_t;

    ////////////////////////////////////////////////////////////
    // not_fn_t

    template<typename F>
    struct not_fn_t
    {
        private:

            F func;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            not_fn_t() = default;

            explicit constexpr not_fn_t(const F& func):
                func(func)
            {}

            explicit constexpr not_fn_t(F&& func):
                func(std::move(func))
            {}

            ////////////////////////////////////////////////////////////
            // Call

            template<typename... Args>
            constexpr auto operator()(Args&&... args) &
                noexcept(noexcept(not utility::as_function(func)(std::forward<Args>(args)...)))
                -> decltype(not utility::as_function(func)(std::forward<Args>(args)...))
            {
                return not utility::as_function(func)(std::forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) const&
                noexcept(noexcept(not utility::as_function(func)(std::forward<Args>(args)...)))
                -> decltype(not utility::as_function(func)(std::forward<Args>(args)...))
            {
                return not utility::as_function(func)(std::forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) &&
                noexcept(noexcept(not utility::as_function(std::move(func))(std::forward<Args>(args)...)))
                -> decltype(not utility::as_function(std::move(func))(std::forward<Args>(args)...))
            {
                return not utility::as_function(std::move(func))(std::forward<Args>(args)...);
            }

            template<typename... Args>
            constexpr auto operator()(Args&&... args) const&&
                noexcept(noexcept(not utility::as_function(std::move(func))(std::forward<Args>(args)...)))
                -> decltype(not utility::as_function(std::move(func))(std::forward<Args>(args)...))
            {
                return not utility::as_function(std::move(func))(std::forward<Args>(args)...);
            }

            ////////////////////////////////////////////////////////////
            // Accessor

            constexpr auto base() const
                -> F
            {
                return func;
            }
    };

    ////////////////////////////////////////////////////////////
    // Helper for not_fn_t construction

    namespace detail
    {
        template<typename F>
        struct not_fn_impl
        {
            using type = not_fn_t<F>;

            static constexpr auto construct(const F& func)
                -> type
            {
                return type(func);
            }

            static constexpr auto construct(F&& func)
                -> type
            {
                return type(std::move(func));
            }
        };

        template<typename F>
        struct not_fn_impl<not_fn_t<F>>
        {
            using type = F;

            static constexpr auto construct(const not_fn_t<F>& func)
                -> type
            {
                return func.base();
            }

            static constexpr auto construct(not_fn_t<F>&& func)
                -> type
            {
                return std::move(func).base();
            }
        };

        template<typename F>
        struct not_fn_impl<flip_t<not_fn_t<F>>>
        {
            using type = flip_t<F>;

            static constexpr auto construct(const flip_t<not_fn_t<F>>& func)
                -> type
            {
                return type(func.base().base());
            }

            static constexpr auto construct(flip_t<not_fn_t<F>>&& func)
                -> type
            {
                return type(std::move(func).base().base());
            }
        };
    }

    ////////////////////////////////////////////////////////////
    // C++17 std::not_fn equivalent

    template<typename F>
    constexpr auto not_fn(F&& func)
        -> typename detail::not_fn_impl<std::decay_t<F>>::type
    {
        return detail::not_fn_impl<std::decay_t<F>>::construct(std::forward<F>(func));
    }
}

#endif // CPPSORT_COMPARATORS_NOT_FN_H_
