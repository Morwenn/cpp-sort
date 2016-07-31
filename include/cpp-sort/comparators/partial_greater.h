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
#ifndef CPPSORT_COMPARATORS_PARTIAL_GREATER_H_
#define CPPSORT_COMPARATORS_PARTIAL_GREATER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/weak_greater.h>
#include <cpp-sort/utility/static_const.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Weak order for floating point types

        template<typename T>
        auto partial_greater(T lhs, T rhs)
            -> std::enable_if_t<std::is_floating_point<T>::value, bool>
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
        };
    }

    namespace
    {
        constexpr auto&& partial_greater = utility::static_const<
            detail::partial_greater_fn
        >::value;
    }
}

#endif // CPPSORT_COMPARATORS_PARTIAL_GREATER_H_
