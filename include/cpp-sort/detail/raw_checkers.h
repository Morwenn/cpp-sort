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
#ifndef CPPSORT_DETAIL_RAW_CHECKERS_H_
#define CPPSORT_DETAIL_RAW_CHECKERS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/utility/any_all.h>
#include <cpp-sort/utility/detection.h>

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Raw checkers (check with the type itself)

    template<typename T, typename=void>
    struct has_iterator_category:
        std::false_type
    {};

    template<typename T>
    struct has_iterator_category<T, utility::void_t<typename T::iterator_category>>:
        std::true_type
    {};

    template<bool, typename...>
    struct raw_check_iterator_category_impl {};

    template<typename... Sorters>
    struct raw_check_iterator_category_impl<true, Sorters...>
    {
        using iterator_category = std::common_type_t<
            typename Sorters::iterator_category...
        >;
    };

    template<typename... Sorters>
    struct raw_check_iterator_category:
        raw_check_iterator_category_impl<
            utility::all(has_iterator_category<Sorters>::value...),
            Sorters...
        >
    {};

    template<typename T, typename=void>
    struct has_is_always_stable:
        std::false_type
    {};

    template<typename T>
    struct has_is_always_stable<T, utility::void_t<typename T::is_always_stable>>:
        std::true_type
    {};

    template<bool, typename...>
    struct raw_check_is_always_stable_impl {};

    template<typename... Sorters>
    struct raw_check_is_always_stable_impl<true, Sorters...>
    {
        using is_always_stable = std::integral_constant<
            bool,
            utility::all(typename Sorters::is_always_stable{}()...)
        >;
    };

    template<typename... Sorters>
    struct raw_check_is_always_stable:
        raw_check_is_always_stable_impl<
            utility::all(has_is_always_stable<Sorters>::value...),
            Sorters...
        >
    {};
}}

#endif // CPPSORT_DETAIL_RAW_CHECKERS_H_
