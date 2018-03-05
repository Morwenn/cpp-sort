/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
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
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Overload resolution tool

        template<std::size_t Value>
        struct choice:
            choice<Value - 1>
        {};

        template<>
        struct choice<0> {};

        ////////////////////////////////////////////////////////////
        // Associate a priority to iterator categories, there is
        // probably a trick to automate that...

        template<typename>
        std::size_t iterator_category_value;

        template<>
        constexpr std::size_t iterator_category_value<std::random_access_iterator_tag> = 3;

        template<>
        constexpr std::size_t iterator_category_value<std::bidirectional_iterator_tag> = 2;

        template<>
        constexpr std::size_t iterator_category_value<std::forward_iterator_tag> = 1;

        template<>
        constexpr std::size_t iterator_category_value<std::input_iterator_tag> = 0;

        // Avoid just a bit of redundancy
        template<typename Iterator, std::size_t N>
        using choice_for_it = choice<
            (iterator_category_value<
                iterator_category_t<Iterator>
            > + 1) * N - 1
        >;

        ////////////////////////////////////////////////////////////
        // Import every operator() in one class

        template<typename Head, typename... Tail>
        struct sorters_merger:
            Head, sorters_merger<Tail...>
        {
            using Head::operator();
            using Head::_detail_stability;

            using sorters_merger<Tail...>::operator();
            using sorters_merger<Tail...>::_detail_stability;
        };

        template<typename Head>
        struct sorters_merger<Head>:
            Head
        {
            using Head::operator();
            using Head::_detail_stability;
        };

        ////////////////////////////////////////////////////////////
        // Add a dispatch to the operator() so that a sorter is
        // preferred for its iterator category first, then for its
        // position into the sorters

        template<typename Sorter, std::size_t Ind>
        struct selection_wrapper
        {
            template<typename... Args>
            auto operator()(choice<Ind>, Args&&... args) const
                -> decltype(Sorter{}(std::forward<Args>(args)...))
            {
                return Sorter{}(std::forward<Args>(args)...);
            }

            template<typename... Args>
            static auto _detail_stability(choice<Ind>, Args&&... args)
                -> std::enable_if_t<
                    is_invocable_v<Sorter(Args...)>,
                    is_stable<Sorter(Args...)>
                >;
        };
    }

    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename... Sorters>
    struct hybrid_adapter:
        public detail::check_iterator_category<Sorters...>,
        public detail::check_is_always_stable<Sorters...>,
        public sorter_facade_fptr<hybrid_adapter<Sorters...>>
    {
        public:

            hybrid_adapter() = default;

            // Automatic deduction guide
            constexpr hybrid_adapter(Sorters...) noexcept {};

        private:

            // Associate and index to every sorter depending on
            // its position in the parameter pack
            template<typename>
            struct dispatch_sorter_impl;

            template<std::size_t... Indices>
            struct dispatch_sorter_impl<std::index_sequence<Indices...>>
            {
                using type = detail::sorters_merger<
                    detail::selection_wrapper<
                        Sorters,
                        sizeof...(Sorters) * detail::iterator_category_value<iterator_category<Sorters>>
                        + sizeof...(Indices) - Indices - 1

                    >...
                >;
            };

            // Dispatch-enabled sorter
            using dispatch_sorter = typename dispatch_sorter_impl<
                std::make_index_sequence<sizeof...(Sorters)>
            >::type;

        public:

            template<typename Iterable, typename... Args>
            auto operator()(Iterable&& iterable, Args&&... args) const
                -> decltype(dispatch_sorter{}(
                    detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                    std::forward<Iterable>(iterable),
                    std::forward<Args>(args)...
                ))
            {
                // Call the appropriate operator()
                return dispatch_sorter{}(
                    detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                    std::forward<Iterable>(iterable), std::forward<Args>(args)...
                );
            }

            template<typename Iterator, typename... Args>
            auto operator()(Iterator first, Iterator last, Args&&... args) const
                -> decltype(dispatch_sorter{}(
                        detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                        std::move(first), std::move(last),
                        std::forward<Args>(args)...
                ))
            {
                // Call the appropriate operator()
                return dispatch_sorter{}(
                    detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                    std::move(first), std::move(last), std::forward<Args>(args)...
                );
            }

            template<typename Iterable, typename... Args>
            static auto _detail_stability(Iterable&& iterable, Args&&... args)
                -> decltype(dispatch_sorter::_detail_stability(
                    detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                    std::forward<Iterable>(iterable),
                    std::forward<Args>(args)...
                ));

            template<typename Iterator, typename... Args>
            static auto _detail_stability(Iterator first, Iterator last, Args&&... args)
                -> decltype(dispatch_sorter::_detail_stability(
                        detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                        std::move(first), std::move(last),
                        std::forward<Args>(args)...
                ));
    };

    ////////////////////////////////////////////////////////////
    // is_stable specializations

    template<typename... Sorters, typename... Args>
    struct is_stable<hybrid_adapter<Sorters...>(Args...)>:
        decltype(hybrid_adapter<Sorters...>::_detail_stability(std::declval<Args&>()...))
    {};
}

#endif // CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_
