/*
 * Copyright (c) 2021 Morwenn
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
    ////////////////////////////////////////////////////////////
    // C++17 std::not_fn

    template<typename F>
    struct not_fn_t
    {
        private:

            F func;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

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
    };

    template<typename F>
    constexpr auto not_fn(F&& func)
        -> not_fn_t<std::decay_t<F>>
    {
        return not_fn_t<std::decay_t<F>>(std::forward<F>(func));
    }
}

#endif // CPPSORT_COMPARATORS_NOT_FN_H_
