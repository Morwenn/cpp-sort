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
#ifndef CPPSORT_REFINED_H_
#define CPPSORT_REFINED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Whether a comparator has a member refine method

        template<typename Compare, typename T>
        using has_refine_method_t
            = decltype(std::declval<Compare&>().template refine<T>());

        template<typename Compare, typename T>
        constexpr bool has_refine_method
            = utility::is_detected_v<has_refine_method_t, Compare, T>;
    }

    ////////////////////////////////////////////////////////////
    // Return a refined comparison function if possible else
    // the original comparison function

    template<
        typename T,
        typename Compare,
        typename = std::enable_if_t<detail::has_refine_method<Compare, T>>
    >
    auto refined(Compare compare)
        noexcept(noexcept(compare.template refine<T>()))
        -> decltype(compare.template refine<T>())
    {
        return compare.template refine<T>();
    }

    template<
        typename T,
        typename Compare,
        typename = std::enable_if_t<not detail::has_refine_method<Compare, T>>
    >
    auto refined(Compare compare)
        noexcept(noexcept(compare))
        -> decltype(compare)
    {
        return compare;
    }

    template<typename T, typename Compare>
    using refined_t = std::decay_t<decltype(refined<std::decay_t<T>>(std::declval<Compare&>()))>;
}

#endif // CPPSORT_REFINED_H_
