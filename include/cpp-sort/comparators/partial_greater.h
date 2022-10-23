/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_PARTIAL_GREATER_H_
#define CPPSORT_COMPARATORS_PARTIAL_GREATER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <concepts>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/weak_greater.h>
#include <cpp-sort/utility/branchless_traits.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Weak order for floating point types

        template<std::floating_point T>
        auto partial_greater(T lhs, T rhs) noexcept
            -> bool
        {
            return lhs > rhs;
        }

        ////////////////////////////////////////////////////////////
        // Generic overload: a weak order is also a partial order

        template<typename T>
        auto partial_greater(const T& lhs, const T& rhs)
            noexcept(noexcept(cppsort::weak_greater(lhs, rhs)))
            -> decltype(cppsort::weak_greater(lhs, rhs))
        {
            return cppsort::weak_greater(lhs, rhs);
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct partial_greater_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(partial_greater(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(partial_greater(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return partial_greater(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using partial_greater_t = detail::partial_greater_fn;
    inline constexpr partial_greater_t partial_greater{};

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::partial_greater_t, T>:
            std::is_arithmetic<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_PARTIAL_GREATER_H_
