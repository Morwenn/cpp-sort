/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#ifndef CPPSORT_REFINED_H_
#define CPPSORT_REFINED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include "detail/detection.h"
#include "detail/remove_cvref.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Whether a comparator has a member refine method

        template<typename Function, typename T>
        using has_refine_method_t
            = decltype(std::declval<Function&>().template refine<T>());

        template<typename Function, typename T>
        constexpr bool has_refine_method
            = is_detected_v<has_refine_method_t, Function, T>;
    }

    ////////////////////////////////////////////////////////////
    // Return a refined comparison function if possible else
    // the original comparison function

    template<
        typename T,
        typename Function,
        typename = std::enable_if_t<detail::has_refine_method<Function, T>>
    >
    auto refined(Function func)
        noexcept(noexcept(func.template refine<T>()))
        -> decltype(func.template refine<T>())
    {
        return func.template refine<T>();
    }

    template<
        typename T,
        typename Function,
        typename = std::enable_if_t<not detail::has_refine_method<Function, T>>
    >
    constexpr auto refined(Function&& func) noexcept
        -> Function&&
    {
        return std::forward<Function>(func);
    }

    template<typename T, typename Function>
    using refined_t = detail::remove_cvref_t<
        decltype(refined<std::decay_t<T>>(std::declval<Function&>()))
    >;
}

#endif // CPPSORT_REFINED_H_
