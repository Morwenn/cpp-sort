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
#ifndef CPPSORT_DETAIL_CHECKERS_H_
#define CPPSORT_DETAIL_CHECKERS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include "any_all.h"
#include "raw_checkers.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // High-level checkers (check with sorter_traits)

    template<bool, typename...>
    struct check_iterator_category_impl {};

    template<typename... Sorters>
    struct check_iterator_category_impl<true, Sorters...>
    {
        using iterator_category = std::common_type_t<
            typename sorter_traits<Sorters>::iterator_category...
        >;
    };

    template<typename... Sorters>
    struct check_iterator_category:
        check_iterator_category_impl<
            all(has_iterator_category<sorter_traits<Sorters>>::value...),
            Sorters...
        >
    {};

    template<bool, typename...>
    struct check_is_always_stable_impl {};

    template<typename... Sorters>
    struct check_is_always_stable_impl<true, Sorters...>
    {
        using is_always_stable = std::integral_constant<
            bool,
            all(typename sorter_traits<Sorters>::is_always_stable{}()...)
        >;
    };

    template<typename... Sorters>
    struct check_is_always_stable:
        check_is_always_stable_impl<
            all(has_is_always_stable<sorter_traits<Sorters>>::value...),
            Sorters...
        >
    {};
}}

#endif // CPPSORT_DETAIL_CHECKERS_H_
