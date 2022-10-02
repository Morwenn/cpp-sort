/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
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
    // Useful forward declaration
    template<typename... Sorters>
    struct hybrid_adapter;

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

        namespace // GitHub issue #158
        {
            template<typename>
            std::size_t iterator_category_value;

            template<>
            constexpr std::size_t iterator_category_value<std::random_access_iterator_tag> = 2;

            template<>
            constexpr std::size_t iterator_category_value<std::bidirectional_iterator_tag> = 1;

            template<>
            constexpr std::size_t iterator_category_value<std::forward_iterator_tag> = 0;
        }

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
            constexpr auto operator()(choice<Ind>, Args&&... args) const
                -> decltype(this->get()(std::forward<Args>(args)...))
            {
                return this->get()(std::forward<Args>(args)...);
            }

            template<typename... Args>
            static constexpr auto _detail_stability(choice<Ind>, Args&&... args)
                -> detail::enable_if_t<
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
            // Access a sorter by index

            template<std::size_t N>
            constexpr auto get() &
                -> decltype(auto)
            {
                using sorter_t = std::tuple_element_t<N, std::tuple<Sorters...>>;

                return hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<sorter_t>>
                    + sizeof...(Indices) - N - 1,
                    sorter_t
                >::get();
            }

            template<std::size_t N>
            constexpr auto get() const&
                -> decltype(auto)
            {
                using sorter_t = std::tuple_element_t<N, std::tuple<Sorters...>>;

                return hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<sorter_t>>
                    + sizeof...(Indices) - N - 1,
                    sorter_t
                >::get();
            }

            template<std::size_t N>
            constexpr auto get() &&
                -> decltype(auto)
            {
                using sorter_t = std::tuple_element_t<N, std::tuple<Sorters...>>;

                return hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<sorter_t>>
                    + sizeof...(Indices) - N - 1,
                    sorter_t
                >::get();
            }

            template<std::size_t N>
            constexpr auto get() const&&
                -> decltype(auto)
            {
                using sorter_t = std::tuple_element_t<N, std::tuple<Sorters...>>;

                return hybrid_adapter_storage_leaf<
                    sizeof...(Sorters) * iterator_category_value<iterator_category<sorter_t>>
                    + sizeof...(Indices) - N - 1,
                    sorter_t
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

                template<std::size_t... Indices>
                constexpr explicit hybrid_adapter_impl(std::tuple<Sorters&&...>&& sorters,
                                                       std::index_sequence<Indices...>):
                    base_class(std::move(std::get<Indices>(sorters))...)
                {}

                template<std::size_t... Indices>
                constexpr explicit hybrid_adapter_impl(std::tuple<Sorters&&...>&& sorters):
                    hybrid_adapter_impl(std::move(sorters), std::make_index_sequence<sizeof...(Sorters)>{})
                {}

                ////////////////////////////////////////////////////////////
                // Call operator

                template<typename Iterable, typename... Args>
                constexpr auto operator()(Iterable&& iterable, Args&&... args) const
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
                constexpr auto operator()(Iterator first, Iterator last, Args&&... args) const
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
                static constexpr auto _detail_stability(Iterable&& iterable, Args&&... args)
                    -> decltype(base_class::_detail_stability(
                        detail::choice_for_it<decltype(std::begin(iterable)), sizeof...(Sorters)>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    ));

                template<typename Iterator, typename... Args>
                static constexpr auto _detail_stability(Iterator first, Iterator last, Args&&... args)
                    -> decltype(base_class::_detail_stability(
                            detail::choice_for_it<Iterator, sizeof...(Sorters)>{},
                            std::move(first), std::move(last),
                            std::forward<Args>(args)...
                    ));
        };

        ////////////////////////////////////////////////////////////
        // Mechanism used to unwrap nested hybrid_adapter

        template<template<typename...> class Flattenable, typename TypeList, typename Accumulator>
        struct flatten_fold;

        template<
            template<typename...> class Flattenable,
            template<typename...> class TsList, typename Front, typename... Rest,
            typename... As
        >
        struct flatten_fold<Flattenable, TsList<Front, Rest...>, hybrid_adapter_impl<As...>>
        {
            using type = typename flatten_fold<Flattenable, TsList<Rest...>, hybrid_adapter_impl<As..., Front>>::type;
        };

        template<
            template<typename...> class Flattenable,
            template<typename...> class TsList, typename... InnerTs, typename... Rest,
            typename... As
        >
        struct flatten_fold<Flattenable, TsList<Flattenable<InnerTs...>, Rest...>, hybrid_adapter_impl<As...>>
        {
            using type = typename flatten_fold<Flattenable, TsList<InnerTs..., Rest...>, hybrid_adapter_impl<As...>>::type;
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
    {
        private:

            using base_class = typename detail::flatten_fold<
                ::cppsort::hybrid_adapter,
                hybrid_adapter<Sorters...>,
                detail::hybrid_adapter_impl<>
            >::type;

            template<typename... Args, std::size_t... Indices>
            static constexpr auto get_sorters_from_impl(detail::hybrid_adapter_impl<Args...>&& value,
                                                        std::index_sequence<Indices...>)
                -> decltype(auto)
            {
                return std::forward_as_tuple(value.template get<Indices>()...);
            }

            template<typename... Args>
            static constexpr auto get_sorters_from_impl(detail::hybrid_adapter_impl<Args...>&& value)
                -> decltype(auto)
            {
                return get_sorters_from_impl(std::move(value), std::make_index_sequence<sizeof...(Args)>{});
            }

            template<typename Sorter>
            static constexpr auto get_flat_tuple(Sorter&& value)
                -> detail::enable_if_t<
                    not detail::is_specialization_of_v<detail::remove_cvref_t<Sorter>, cppsort::hybrid_adapter>,
                    std::tuple<std::remove_reference_t<Sorter>&&>
                >
            {
                return std::forward_as_tuple(std::move(value));
            }

            template<typename Sorter>
            static constexpr auto get_flat_tuple(Sorter&& value)
                -> detail::enable_if_t<
                    detail::is_specialization_of_v<detail::remove_cvref_t<Sorter>, cppsort::hybrid_adapter>,
                    decltype(get_sorters_from_impl(std::move(value)))
                >
            {
                return get_sorters_from_impl(std::move(value));
            }

        public:

            hybrid_adapter() = default;

            constexpr hybrid_adapter(Sorters... sorters):
                base_class(std::tuple_cat(get_flat_tuple(std::move(sorters))...))
            {}
    };

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
