/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_TOTAL_GREATER_H_
#define CPPSORT_COMPARATORS_TOTAL_GREATER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <concepts>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/concepts.h>
#include <cpp-sort/utility/branchless_traits.h>
#include "../detail/floating_point_weight.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Total order for integral types

        template<mstd::integral T>
        constexpr auto total_greater(T lhs, T rhs) noexcept
            -> bool
        {
            return lhs > rhs;
        }

        ////////////////////////////////////////////////////////////
        // Total order for floating point types

        template<std::floating_point T>
        auto total_greater(T lhs, T rhs)
            -> bool
        {
            if (std::isfinite(lhs) && std::isfinite(rhs)) {
                if (lhs == 0 && rhs == 0) {
                    return std::signbit(rhs) && not std::signbit(lhs);
                }
                return lhs > rhs;
            }

            int lhs_weight = total_weight(lhs);
            int rhs_weight = total_weight(rhs);
            return lhs_weight > rhs_weight;
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct total_greater_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(total_greater(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(total_greater(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return total_greater(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using total_greater_t = detail::total_greater_fn;
    inline constexpr total_greater_t total_greater{};

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::total_greater_t, T>:
            std::is_integral<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_TOTAL_GREATER_H_
