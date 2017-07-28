/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
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
#ifndef CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_
#define CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include "../detail/logical_traits.h"

namespace cppsort
{
namespace utility
{
    ////////////////////////////////////////////////////////////
    // Whether a comparison function is likely branchless when
    // called with instances of a specific type

    namespace detail
    {
        template<typename Compare, typename T>
        struct is_probably_branchless_comparison_impl:
            std::false_type
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::less<>, T>:
            std::is_arithmetic<T>
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::less<T>, T>:
            std::is_arithmetic<T>
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::greater<>, T>:
            std::is_arithmetic<T>
        {};

        template<typename T>
        struct is_probably_branchless_comparison_impl<std::greater<T>, T>:
            std::is_arithmetic<T>
        {};
    }

    // Strip types from cv and reference qualifications if needed

    template<typename Compare, typename T>
    struct is_probably_branchless_comparison:
        std::conditional_t<
            cppsort::detail::disjunction<
                std::is_reference<Compare>,
                std::is_const<Compare>,
                std::is_volatile<Compare>,
                std::is_reference<T>,
                std::is_const<T>,
                std::is_volatile<T>
            >::value,
            is_probably_branchless_comparison<
                std::remove_cv_t<std::remove_reference_t<Compare>>,
                std::remove_cv_t<std::remove_reference_t<T>>
            >,
            detail::is_probably_branchless_comparison_impl<Compare, T>
        >
    {};

    template<typename Compare, typename T>
    constexpr bool is_probably_branchless_comparison_v
        = is_probably_branchless_comparison<Compare, T>::value;

    ////////////////////////////////////////////////////////////
    // Whether a projection function is likely branchless when
    // called with an instance of a specific type

    namespace detail
    {
        template<typename Projection, typename T>
        struct is_probably_branchless_projection_impl:
            std::is_member_object_pointer<Projection>
        {};
    }

    // Strip types from cv and reference qualifications if needed

    template<typename Projection, typename T>
    struct is_probably_branchless_projection:
        std::conditional_t<
            cppsort::detail::disjunction<
                std::is_reference<Projection>,
                std::is_const<Projection>,
                std::is_volatile<Projection>,
                std::is_reference<T>,
                std::is_const<T>,
                std::is_volatile<T>
            >::value,
            is_probably_branchless_projection<
                std::remove_cv_t<std::remove_reference_t<Projection>>,
                std::remove_cv_t<std::remove_reference_t<T>>
            >,
            detail::is_probably_branchless_projection_impl<Projection, T>
        >
    {};

    template<typename Projection, typename T>
    constexpr bool is_probably_branchless_projection_v
        = is_probably_branchless_projection<Projection, T>::value;
}}

#endif // CPPSORT_UTILITY_BRANCHLESS_TRAITS_H_
