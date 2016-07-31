/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#ifndef CPPSORT_COMPARATORS_TOTAL_GREATER_H_
#define CPPSORT_COMPARATORS_TOTAL_GREATER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cmath>
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/static_const.h>
#include "../detail/floating_point_weight.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Total order for integral types

        template<typename T>
        constexpr auto total_greater(T lhs, T rhs) noexcept
            -> std::enable_if_t<std::is_integral<T>::value, bool>
        {
            return lhs > rhs;
        }

        ////////////////////////////////////////////////////////////
        // Total order for floating point types

        template<typename T>
        auto total_greater(T lhs, T rhs)
            -> std::enable_if_t<std::is_floating_point<T>::value, bool>
        {
            if (std::isfinite(lhs) && std::isfinite(rhs))
            {
                if (lhs == 0 && rhs == 0)
                {
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
        };
    }

    namespace
    {
        constexpr auto&& total_greater = utility::static_const<
            detail::total_greater_fn
        >::value;
    }
}

#endif // CPPSORT_COMPARATORS_TOTAL_GREATER_H_
