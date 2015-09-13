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
#ifndef CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_
#define CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_base.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/any_all.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename... Sorters>
    class hybrid_adapter:
        sorter_base<hybrid_adapter<Sorters...>>
    {
        private:

            ////////////////////////////////////////////////////////////
            // Import every operator() in one class

            template<typename Head, typename... Tail>
            struct sorters_merger:
                Head, sorters_merger<Tail...>
            {
                using Head::operator();
                using sorters_merger<Tail...>::operator();
            };

            template<typename Head>
            struct sorters_merger<Head>:
                Head
            {
                using Head::operator();
            };

            ////////////////////////////////////////////////////////////
            // Add a dispatch on the iterator category

            template<typename Sorter>
            struct category_wrapper
            {
                template<typename... Args>
                auto operator()(iterator_category<Sorter>, Args&&... args) const
                    -> decltype(auto)
                {
                    return Sorter{}(std::forward<Args>(args)...);
                }
            };

            // Dispatch-enabled sorter
            using dispatch_sorter = sorters_merger<category_wrapper<Sorters>...>;

        public:

            template<typename Iterable, typename... Args>
            auto operator()(Iterable& iterable, Args&&... args) const
                -> decltype(auto)
            {
                // Iterator category of the iterable to sort
                using category =
                    typename std::iterator_traits<
                        decltype(std::begin(iterable))
                    >::iterator_category;

                // Call the appropriate operator()
                return dispatch_sorter{}(category{}, iterable, std::forward<Args>(args)...);
            }

            template<typename Iterator, typename... Args>
            auto operator()(Iterator first, Iterator last, Args&&... args) const
                -> decltype(auto)
            {
                // Iterator category of the iterable to sort
                using category = typename std::iterator_traits<Iterator>::iterator_category;

                // Call the appropriate operator()
                return dispatch_sorter{}(category{}, first, last, std::forward<Args>(args)...);
            }
    };

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename... Sorters>
    struct sorter_traits<hybrid_adapter<Sorters...>>
    {
        // The iterator category is the least constrained one
        // among the aggregated sorters
        using iterator_category = std::common_type_t<iterator_category<Sorters>...>;

        // The adapter is stable only if every aggregated sorter
        // is stable
        static constexpr bool is_stable = utility::all(sorter_traits<Sorters>::is_stable...);
    };
}

#endif // CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_
