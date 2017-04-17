/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Weak order for floating point types

        template<typename T>
        auto weak_greater(T lhs, T rhs)
            -> std::enable_if_t<std::is_floating_point<T>::value, bool>
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
        };
    }

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
        struct is_probably_branchless_comparison<decltype(weak_greater), T>:
            std::is_integral<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_WEAK_GREATER_H_
