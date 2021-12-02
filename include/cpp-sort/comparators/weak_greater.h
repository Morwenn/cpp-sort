/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_COMPARATORS_WEAK_GREATER_H_
#define CPPSORT_COMPARATORS_WEAK_GREATER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/total_greater.h>
#include <cpp-sort/utility/branchless_traits.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/floating_point_weight.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Weak order for floating point types

        template<typename T>
        auto weak_greater(T lhs, T rhs)
            -> detail::enable_if_t<std::is_floating_point<T>::value, bool>
        {
            if (std::isfinite(lhs) && std::isfinite(rhs)) {
                return lhs > rhs;
            }

            int lhs_weight = weak_weight(lhs);
            int rhs_weight = weak_weight(rhs);
            return lhs_weight > rhs_weight;
        }

        ////////////////////////////////////////////////////////////
        // Generic overload: a total order is also a weak order

        template<typename T>
        auto weak_greater(const T& lhs, const T& rhs)
            noexcept(noexcept(cppsort::total_greater(lhs, rhs)))
            -> decltype(cppsort::total_greater(lhs, rhs))
        {
            return cppsort::total_greater(lhs, rhs);
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct weak_greater_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(weak_greater(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(weak_greater(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return weak_greater(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using weak_greater_t = detail::weak_greater_fn;

    namespace
    {
        constexpr auto&& weak_greater = utility::static_const<
            detail::weak_greater_fn
        >::value;
    }

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::weak_greater_t, T>:
            std::is_integral<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_WEAK_GREATER_H_
