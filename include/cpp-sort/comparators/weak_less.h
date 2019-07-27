/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2019 Morwenn
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
#ifndef CPPSORT_COMPARATORS_WEAK_LESS_H_
#define CPPSORT_COMPARATORS_WEAK_LESS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/total_less.h>
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
        auto weak_less(T lhs, T rhs)
            -> std::enable_if_t<std::is_floating_point<T>::value, bool>
        {
            if (std::isfinite(lhs) && std::isfinite(rhs)) {
                return lhs < rhs;
            }

            int lhs_weight = weak_weight(lhs);
            int rhs_weight = weak_weight(rhs);
            return lhs_weight < rhs_weight;
        }

        ////////////////////////////////////////////////////////////
        // Generic overload: a total order is also a weak order

        template<typename T>
        auto weak_less(const T& lhs, const T& rhs)
            noexcept(noexcept(cppsort::total_less(lhs, rhs)))
            -> decltype(cppsort::total_less(lhs, rhs))
        {
            return cppsort::total_less(lhs, rhs);
        }

        ////////////////////////////////////////////////////////////
        // Customization point

        struct weak_less_fn
        {
            template<typename T, typename U>
            constexpr auto operator()(T&& lhs, U&& rhs) const
                noexcept(noexcept(weak_less(std::forward<T>(lhs), std::forward<U>(rhs))))
                -> decltype(weak_less(std::forward<T>(lhs), std::forward<U>(rhs)))
            {
                return weak_less(std::forward<T>(lhs), std::forward<U>(rhs));
            }

            using is_transparent = void;
        };
    }

    using weak_less_t = detail::weak_less_fn;

    namespace
    {
        constexpr auto&& weak_less = utility::static_const<
            detail::weak_less_fn
        >::value;
    }

    // Branchless traits

    namespace utility
    {
        template<typename T>
        struct is_probably_branchless_comparison<cppsort::weak_less_t, T>:
            std::is_integral<T>
        {};
    }
}

#endif // CPPSORT_COMPARATORS_WEAK_LESS_H_
