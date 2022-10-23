/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_PARTIAL_LESS_H_
#define CPPSORT_COMPARATORS_PARTIAL_LESS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/weak_less.h>
#include <cpp-sort/utility/branchless_traits.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Weak order for floating point types

        template<std::floating_point T>
        auto partial_less(T lhs, T rhs) noexcept
            -> bool
        {
            return lhs < rhs;
        }

        ////////////////////////////////////////////////////////////
        // Generic overload: a weak order is also a partial order

        template<typename T>
        auto partial_less(const T& lhs, const T& rhs)
            noexcept(noexcept(cppsort::weak_less(lhs, rhs)))
            -> decltype(cppsort::weak_less(lhs, rhs))
        {
            return cppsort::weak_less(lhs, rhs);
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct partial_less_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(partial_less(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(partial_less(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return partial_less(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using partial_less_t = detail::partial_less_fn;
    inline constexpr partial_less_t partial_less{};

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::partial_less_t, T>:
            std::is_arithmetic<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_PARTIAL_LESS_H_
