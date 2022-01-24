/*
 * Copyright (c) 2021-2022 Morwenn
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
#include "../detail/raw_checkers.h"

namespace cppsort
{
    template<typename F>
    struct not_fn_t;
    template<typename F1, typename F2>
    class projection_compare;

    ////////////////////////////////////////////////////////////
    // flip_t

    template<typename F>
    struct flip_t:
        detail::raw_check_is_transparent<F>
    {
        private:

            F func;

        public:

            ////////////////////////////////////////////////////////////
            // Construction

            flip_t() = default;

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

            ////////////////////////////////////////////////////////////
            // Accessor

            constexpr auto base() const
                -> F
            {
                return func;
            }
    };

    ////////////////////////////////////////////////////////////
    // Helper for flip_t construction

    namespace detail
    {
        template<typename F>
        struct flip_impl
        {
            using type = flip_t<F>;

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
        struct flip_impl<flip_t<F>>
        {
            using type = F;

            static constexpr auto construct(const flip_t<F>& func)
                -> type
            {
                return func.base();
            }

            static constexpr auto construct(flip_t<F>&& func)
                -> type
            {
                return std::move(func).base();
            }
        };

        template<typename F>
        struct flip_impl<not_fn_t<flip_t<F>>>
        {
            using type = not_fn_t<F>;

            static constexpr auto construct(const not_fn_t<flip_t<F>>& func)
                -> type
            {
                return type(func.base().base());
            }

            static constexpr auto construct(not_fn_t<flip_t<F>>&& func)
                -> type
            {
                return type(std::move(func).base().base());
            }
        };

        template<typename F1, typename F2>
        struct flip_impl<projection_compare<F1, F2>>
        {
            using type = projection_compare<
                typename detail::flip_impl<F1>::type, F2
            >;

            static constexpr auto construct(const projection_compare<F1, F2>& func)
                -> type
            {
                return type(
                    detail::flip_impl<F1>::construct(func.comparison()),
                    func.projection()
                );
            }

            static constexpr auto construct(projection_compare<F1, F2>&& func)
                -> type
            {
                return type(
                    detail::flip_impl<F1>::construct(std::move(func.comparison())),
                    std::move(func.projection())
                );
            }
        };
    }

    ////////////////////////////////////////////////////////////
    // flip
    //
    // Name taken from Haskell flip function from the Prelude
    // module: it takes a binary Callable and returns another
    // Callable with its arguments flipped.

    template<typename F>
    constexpr auto flip(F&& func)
        -> typename detail::flip_impl<std::decay_t<F>>::type
    {
        return detail::flip_impl<std::decay_t<F>>::construct(std::forward<F>(func));
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
