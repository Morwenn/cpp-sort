/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
#define CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_base.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/has_sort_method.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename Sorter>
    struct self_sort_adapter:
        sorter_base<self_sort_adapter<Sorter>>
    {
        template<
            typename Iterable,
            typename Compare = std::less<>
        >
        auto operator()(Iterable& iterable, Compare compare={}) const
            -> std::enable_if_t<utility::has_sort_method<Iterable>, void>
        {
            iterable.sort(compare);
        }

        template<
            typename Iterable,
            typename Compare = std::less<>
        >
        auto operator()(Iterable& iterable, Compare compare={}) const
            -> std::enable_if_t<not utility::has_sort_method<Iterable>, void>
        {
            Sorter{}(iterable, compare);
        }
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename Sorter>
    struct sorter_traits<self_sort_adapter<Sorter>>
    {
        using iterator_category = iterator_category<Sorter>;

        // We can't guarantee the stability of the sort method,
        // therefore we default the stability to false
        static constexpr bool is_stable = false;
    };
}

#endif // CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
