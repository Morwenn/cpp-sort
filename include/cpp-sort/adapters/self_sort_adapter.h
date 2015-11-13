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
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/has_sort_method.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<typename Sorter>
        struct self_sort_adapter_impl
        {
            template<
                typename Iterable,
                typename = std::enable_if_t<utility::has_sort_method<Iterable>>
            >
            auto operator()(Iterable& iterable) const
                -> decltype(iterable.sort())
            {
                return iterable.sort();
            }

            template<
                typename Iterable,
                typename = std::enable_if_t<not utility::has_sort_method<Iterable>>,
                typename = void // dummy parameter for ODR
            >
            auto operator()(Iterable& iterable) const
                -> decltype(Sorter{}(iterable))
            {
                return Sorter{}(iterable);
            }

            template<
                typename Iterable,
                typename Compare,
                typename = std::enable_if_t<utility::has_comparison_sort_method<Iterable, Compare>>
            >
            auto operator()(Iterable& iterable, Compare compare) const
                -> decltype(iterable.sort(compare))
            {
                return iterable.sort(compare);
            }

            template<
                typename Iterable,
                typename Compare,
                typename = std::enable_if_t<not utility::has_comparison_sort_method<Iterable, Compare>>,
                typename = void // dummy parameter for ODR
            >
            auto operator()(Iterable& iterable, Compare compare) const
                -> decltype(Sorter{}(iterable, compare))
            {
                return Sorter{}(iterable, compare);
            }

            template<typename Iterator>
            auto operator()(Iterator first, Iterator last) const
                -> decltype(Sorter{}(first, last))
            {
                return Sorter{}(first, last);
            }

            template<typename Iterator, typename Compare>
            auto operator()(Iterator first, Iterator last, Compare compare) const
                -> decltype(Sorter{}(first, last, compare))
            {
                return Sorter{}(first, last, compare);
            }
        };
    }

    template<typename Sorter>
    struct self_sort_adapter:
        sorter_facade<detail::self_sort_adapter_impl<Sorter>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename Sorter>
    struct sorter_traits<self_sort_adapter<Sorter>>
    {
        using iterator_category = cppsort::iterator_category<Sorter>;

        // We can't guarantee the stability of the sort method,
        // therefore we default the stability to false
        static constexpr bool is_stable = false;
    };
}

#endif // CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
