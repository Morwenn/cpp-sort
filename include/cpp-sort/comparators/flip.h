/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_FLIP_H_
#define CPPSORT_COMPARATORS_FLIP_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/branchless_traits.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // flip
    //
    // Name taken from Haskell flip function from the Prelude
    // module: it takes a binary Callable and returns another
    // Callable with its arguments flipped.

    template<typename F>
    struct flip_t
    {
        private:

            F func;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            explicit constexpr flip_t(const F& func):
                func(func)
            {}

            explicit constexpr flip_t(F&& func):
                func(std::move(func))
            {}

            ////////////////////////////////////////////////////////////
            // Call

            template<typename T1, typename T2>
            constexpr auto operator()(T1&& x, T2&& y) &
                noexcept(noexcept(utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x))))
                -> decltype(utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x)))
            {
                return utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x));
            }

            template<typename T1, typename T2>
            constexpr auto operator()(T1&& x, T2&& y) const&
                noexcept(noexcept(utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x))))
                -> decltype(utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x)))
            {
                return utility::as_function(func)(std::forward<T2>(y), std::forward<T1>(x));
            }

            template<typename T1, typename T2>
            constexpr auto operator()(T1&& x, T2&& y) &&
                noexcept(noexcept(utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x))))
                -> decltype(utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x)))
            {
                return utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x));
            }

            template<typename T1, typename T2>
            constexpr auto operator()(T1&& x, T2&& y) const&&
                noexcept(noexcept(utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x))))
                -> decltype(utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x)))
            {
                return utility::as_function(std::move(func))(std::forward<T2>(y), std::forward<T1>(x));
            }
    };

    template<typename F>
    auto flip(F&& func)
        -> flip_t<std::decay_t<F>>
    {
        return flip_t<std::decay_t<F>>(std::forward<F>(func));
    }

    ////////////////////////////////////////////////////////////
    // Branchless traits

    namespace utility
    {
        template<typename F, typename T>
        struct is_probably_branchless_comparison<flip_t<F>, T>:
            is_probably_branchless_comparison<F, T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_FLIP_H_
