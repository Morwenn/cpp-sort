/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2019 Morwenn
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
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/any_all.h"
#include "../detail/checkers.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

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
        // Mechanism used to unwrap nested hybrid_adapter

        template<template<class...> class Flattenable, class TypeList, class Accumulator>
        struct flatten_fold;

        template<
            template<typename...> class Flattenable,
            template<typename...> class TsList, typename Front, typename... Rest,
            template<typename...> class AsList, typename... As
        >
        struct flatten_fold<Flattenable, TsList<Front, Rest...>, AsList<As...>>
        {
            using type = typename flatten_fold<Flattenable, TsList<Rest...>, AsList<As..., Front>>::type;
        };

        template<
            template<typename...> class Flattenable,
            template<typename...> class TsList, typename... InnerTs, typename... Rest,
            template<typename...> class AsList, typename... As
        >
        struct flatten_fold<Flattenable, TsList<Flattenable<InnerTs...>, Rest...>, AsList<As...>>
        {
            using type = typename flatten_fold<Flattenable, TsList<InnerTs..., Rest...>, AsList<As...>>::type;
        };

        template<
            template<typename...> class Flattenable,
            template<typename...> class TsList,
            typename Accumulator
        >
        struct flatten_fold<Flattenable, TsList<>, Accumulator>
        {
            using type = Accumulator;
        };

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
        // hybrid_adapter storage
        //
        // Unlike most adapters, hybrid_adapter actually wraps
        // several sorters, and thus needs a dedicated storage with
        // enhanced capabilities; it also does not provide any
        // operator() because it wouldn't make sense since the
        // storage itself doesn't know which sorter to call

        template<std::size_t Ind, typename Sorter>
        struct hybrid_adapter_storage_leaf:
            utility::adapter_storage<Sorter>
        {
            hybrid_adapter_storage_leaf() = default;

            constexpr hybrid_adapter_storage_leaf(Sorter&& sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {};

            // Add a dispatch to the operator() so that a sorter is
            // preferred for its iterator category first, then for its
            // position into the sorters

            template<typename... Args>
            auto operator()(choice<Ind>, Args&&... args) const
                -> decltype(this->get()(std::forward<Args>(args)...))
            {
                return this->get()(std::forward<Args>(args)...);
            }

            template<typename... Args>
            static auto _detail_stability(choice<Ind>, Args&&... args)
                -> std::enable_if_t<
                    is_invocable_v<Sorter, Args...>,
                    is_stable<Sorter(Args...)>
                >;
        };

        template<typename Head, typename... Tail>
        struct hybrid_adapter_storage_impl:
            Head, hybrid_adapter_storage_impl<Tail...>
        {
            hybrid_adapter_storage_impl() = default;

            constexpr hybrid_adapter_storage_impl(Head&& head, Tail&&... tail):
                Head(std::move(head)),
                hybrid_adapter_storage_impl<Tail...>(std::move(tail)...)
            {};

            using Head::operator();
            using Head::_detail_stability;

            using hybrid_adapter_storage_impl<Tail...>::operator();
            using hybrid_adapter_storage_impl<Tail...>::_detail_stability;
        };

        template<typename Head>
        struct hybrid_adapter_storage_impl<Head>:
            Head
        {
            hybrid_adapter_storage_impl() = default;

            constexpr hybrid_adapter_storage_impl(Head&& head):
                Head(std::move(head))
            {};

            using Head::operator();
            using Head::_detail_stability;
        };

        template<typename Indices, typename... Sorters>
        struct hybrid_adapter_storage;

        template<typename... Sorters, std::size_t... Indices>
        struct hybrid_adapter_storage<std::index_sequence<Indices...>, Sorters...>:
            hybrid_adapter_storage_impl<
                hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<Sorters>>
                    + sizeof...(Indices) - Indices - 1,
                    Sorters
                >...
            >
        {
            using hybrid_adapter_storage_impl<
                hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<Sorters>>
                    + sizeof...(Indices) - Indices - 1,
                    Sorters
                >...
            >::hybrid_adapter_storage_impl;

            ////////////////////////////////////////////////////////////
            // Access a sorter by index & type

            template<std::size_t N, typename Sorter>
            auto get() &&
                -> decltype(auto)
            {
                return hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<Sorter>>
                    + sizeof...(Indices) - N - 1,
                    Sorter
                >::get();
            }
        };

        ////////////////////////////////////////////////////////////
        // Auto-unwrapping adapter implementation

        template<typename... Sorters>
        struct hybrid_adapter_impl:
            detail::hybrid_adapter_storage<std::make_index_sequence<sizeof...(Sorters)>, Sorters...>,
            detail::check_iterator_category<Sorters...>,
            detail::check_is_always_stable<Sorters...>,
            detail::sorter_facade_fptr<
                hybrid_adapter_impl<Sorters...>,
                detail::all(std::is_empty<Sorters>::value...)
            >
        {
            private:

                using base_class = detail::hybrid_adapter_storage<
                    std::make_index_sequence<sizeof...(Sorters)>,
                    Sorters...
                >;

            public:

                ////////////////////////////////////////////////////////////
                // Construction

                hybrid_adapter_impl() = default;

                constexpr explicit hybrid_adapter_impl(Sorters... sorters):
                    base_class(std::move(sorters)...)
                {}

                ////////////////////////////////////////////////////////////
                // Call operator

                template<typename Iterable, typename... Args>
                auto operator()(Iterable&& iterable, Args&&... args) const
                    -> decltype(base_class::operator()(
                        detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    ))
                {
                    return base_class::operator()(
                        detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    );
                }

                template<typename Iterator, typename... Args>
                auto operator()(Iterator first, Iterator last, Args&&... args) const
                    -> decltype(base_class::operator()(
                            detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                            std::move(first), std::move(last),
                            std::forward<Args>(args)...
                    ))
                {
                    return base_class::operator()(
                        detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                        std::move(first), std::move(last),
                        std::forward<Args>(args)...
                    );
                }

                ////////////////////////////////////////////////////////////
                // Stability of a call

                template<typename Iterable, typename... Args>
                static auto _detail_stability(Iterable&& iterable, Args&&... args)
                    -> decltype(base_class::_detail_stability(
                        detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    ));

                template<typename Iterator, typename... Args>
                static auto _detail_stability(Iterator first, Iterator last, Args&&... args)
                    -> decltype(base_class::_detail_stability(
                            detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                            std::move(first), std::move(last),
                            std::forward<Args>(args)...
                    ));
        };
    }

    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename... Sorters>
    struct hybrid_adapter:
        detail::flatten_fold<
            hybrid_adapter,
            hybrid_adapter<Sorters...>,
            detail::hybrid_adapter_impl<>
        >::type
    {};

    ////////////////////////////////////////////////////////////
    // is_stable specializations

    template<typename... Sorters, typename... Args>
    struct is_stable<hybrid_adapter<Sorters...>(Args...)>:
        decltype(hybrid_adapter<Sorters...>::_detail_stability(std::declval<Args&>()...))
    {};
}

#ifdef CPPSORT_ADAPTERS_STABLE_ADAPTER_DONE_
#   include "../detail/stable_adapter_hybrid_adapter.h"
#endif

#define CPPSORT_ADAPTERS_HYBRID_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_
