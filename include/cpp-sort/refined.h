/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_REFINED_H_
#define CPPSORT_REFINED_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include "detail/type_traits.h"

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
        typename = detail::enable_if_t<detail::has_refine_method<Function, T>>
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
        typename = detail::enable_if_t<not detail::has_refine_method<Function, T>>
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
