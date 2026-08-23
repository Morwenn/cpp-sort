/*
 * Copyright (c) 2016-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_TOTAL_LESS_H_
#define CPPSORT_COMPARATORS_TOTAL_LESS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <type_traits>
#include <utility>
#include <version>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/type_traits.h"

#if defined(__cpp_lib_three_way_comparison)
#   include <compare>
#else
#   include <cmath>
#   include "../detail/floating_point_weight.h"
#endif

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Total order for integral types

        template<typename T>
        constexpr auto total_less(T lhs, T rhs) noexcept
            -> detail::enable_if_t<is_integral_v<T>, bool>
        {
            return lhs < rhs;
        }

        ////////////////////////////////////////////////////////////
        // Total order for floating point types

        template<typename T>
        auto total_less(T lhs, T rhs)
            -> detail::enable_if_t<std::is_floating_point_v<T>, bool>
        {
#if defined(__cpp_lib_three_way_comparison)
            // Full implementation of totalOrder
            return std::is_lt(std::strong_order(lhs, rhs));
#else
            // Approximation, unable to differentiate NaNs
            if (std::isfinite(lhs) && std::isfinite(rhs)) {
                if (lhs == 0 && rhs == 0) {
                    return std::signbit(lhs) && not std::signbit(rhs);
                }
                return lhs < rhs;
            }

            int lhs_weight = total_weight(lhs);
            int rhs_weight = total_weight(rhs);
            return lhs_weight < rhs_weight;
#endif
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct total_less_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(total_less(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(total_less(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return total_less(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using total_less_t = detail::total_less_fn;

    inline constexpr detail::total_less_fn total_less{};

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::total_less_t, T>:
            std::is_integral<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_TOTAL_LESS_H_
