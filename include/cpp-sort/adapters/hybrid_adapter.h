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
#include <tuple>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include "../detail/checkers.h"
#include "../detail/is_callable.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Overload resolution tool

        template<std::size_t Value>
        struct choice:
            choice<Value + 1>
        {};

        template<>
        struct choice<127> {};

        ////////////////////////////////////////////////////////////
        // Associate a priority to iterator categories, there is
        // probably a trick to automate that...

        template<typename>
        std::size_t iterator_category_value;

        template<>
        constexpr std::size_t iterator_category_value<std::random_access_iterator_tag> = 0;

        template<>
        constexpr std::size_t iterator_category_value<std::bidirectional_iterator_tag> = 1;

        template<>
        constexpr std::size_t iterator_category_value<std::forward_iterator_tag> = 2;

        template<>
        constexpr std::size_t iterator_category_value<std::input_iterator_tag> = 3;

        // Number of acceptable iterator categories
        constexpr std::size_t categories_number = 4;

        // Avoid just a bit of redundancy
        template<typename Iterator>
        using choice_for_it = choice<
            iterator_category_value<
                iterator_category_t<Iterator>
            > * categories_number
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

            constexpr hybrid_adapter_storage_leaf(Sorter sorter):
                utility::adapter_storage<Sorter>(std::move(sorter))
            {};

            // Add a dispatch to the operator() so that a sorter is
            // preferred for its iterator category first, then for its
            // position into the sorters

            template<typename... Args>
            auto operator()(choice<Ind>, Args&&... args) const
                -> decltype(utility::adapter_storage<Sorter>::get()(std::forward<Args>(args)...))
            {
                return utility::adapter_storage<Sorter>::get()(std::forward<Args>(args)...);
            }

            template<typename... Args>
            auto operator()(choice<Ind>, Args&&... args)
                -> decltype(utility::adapter_storage<Sorter>::get()(std::forward<Args>(args)...))
            {
                return utility::adapter_storage<Sorter>::get()(std::forward<Args>(args)...);
            }

            template<typename... Args>
            static auto detail_stability(choice<Ind>, Args&&... args)
                -> std::enable_if_t<
                    detail::is_callable_v<Sorter(Args...)>,
                    cppsort::is_stable<Sorter(Args...)>
                >
            {
                return {};
            }
        };

        template<typename Head, typename... Tail>
        struct hybrid_adapter_storage_impl:
            Head, hybrid_adapter_storage_impl<Tail...>
        {
            hybrid_adapter_storage_impl() = default;

            constexpr hybrid_adapter_storage_impl(Head head, Tail... tail):
                Head(std::move(head)),
                hybrid_adapter_storage_impl<Tail...>(std::move(tail)...)
            {};

            using Head::operator();
            using Head::detail_stability;

            using hybrid_adapter_storage_impl<Tail...>::operator();
            using hybrid_adapter_storage_impl<Tail...>::detail_stability;
        };

        template<typename Head>
        struct hybrid_adapter_storage_impl<Head>:
            Head
        {
            hybrid_adapter_storage_impl() = default;

            constexpr hybrid_adapter_storage_impl(Head head):
                Head(std::move(head))
            {};

            using Head::operator();
            using Head::detail_stability;
        };

        template<typename Indices, typename... Sorters>
        struct hybrid_adapter_storage;

        template<typename... Sorters, std::size_t... Indices>
        struct hybrid_adapter_storage<std::index_sequence<Indices...>, Sorters...>:
            hybrid_adapter_storage_impl<
                hybrid_adapter_storage_leaf<
                    Indices + iterator_category_value<iterator_category<Sorters>>
                            * categories_number,
                    Sorters
                >...
            >
        {
            hybrid_adapter_storage() = default;

            constexpr hybrid_adapter_storage(Sorters... sorters):
                hybrid_adapter_storage_impl<
                    hybrid_adapter_storage_leaf<
                        Indices + iterator_category_value<iterator_category<Sorters>>
                                * categories_number,
                        Sorters
                    >...
                >(std::move(sorters)...)
            {}
        };

        template<typename... Sorters>
        struct hybrid_adapter_impl_base:
            hybrid_adapter_storage<std::make_index_sequence<sizeof...(Sorters)>, Sorters...>
        {
            hybrid_adapter_impl_base() = default;

            constexpr hybrid_adapter_impl_base(Sorters... sorters):
                hybrid_adapter_storage<
                    std::make_index_sequence<sizeof...(Sorters)>,
                    Sorters...
                >(std::move(sorters)...)
            {}
        };

        ////////////////////////////////////////////////////////////
        // Adapter

        template<typename... Sorters>
        class hybrid_adapter_impl:
            public hybrid_adapter_impl_base<Sorters...>,
            public check_iterator_category<Sorters...>,
            public check_is_always_stable<Sorters...>
        {
            public:

                hybrid_adapter_impl() = default;

                constexpr hybrid_adapter_impl(Sorters... sorters):
                    hybrid_adapter_impl_base<Sorters...>(std::move(sorters)...)
                {}

            private:

                template<typename Self, typename Iterable, typename... Args>
                static auto _call_sorter(Self& self, Iterable&& iterable, Args&&... args)
                    -> decltype(self.template hybrid_adapter_impl_base<Sorters...>::operator()(
                        choice_for_it<decltype(std::begin(iterable))>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    ))
                {
                    // Call the appropriate operator()
                    return self.template hybrid_adapter_impl_base<Sorters...>::operator()(
                        choice_for_it<decltype(std::begin(iterable))>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    );
                }

                template<typename Self, typename Iterator, typename... Args>
                static auto _call_sorter(Self& self, Iterator first, Iterator last, Args&&... args)
                    -> decltype(self.template hybrid_adapter_impl_base<Sorters...>::operator()(
                            choice_for_it<Iterator>{},
                            std::move(first), std::move(last),
                            std::forward<Args>(args)...
                    ))
                {
                    // Call the appropriate operator()
                    return self.template hybrid_adapter_impl_base<Sorters...>::operator()(
                        choice_for_it<Iterator>{},
                        std::move(first), std::move(last),
                        std::forward<Args>(args)...
                    );
                }

            public:

                template<typename... Args>
                auto operator()(Args&&... args) const
                    -> decltype(this->_call_sorter(*this, std::forward<Args>(args)...))
                {
                    return _call_sorter(*this, std::forward<Args>(args)...);
                }

                template<typename... Args>
                auto operator()(Args&&... args)
                    -> decltype(this->_call_sorter(*this, std::forward<Args>(args)...))
                {
                    return _call_sorter(*this, std::forward<Args>(args)...);
                }

                ////////////////////////////////////////////////////////////
                // Stability of a call

                template<typename Iterable, typename... Args>
                static auto detail_stability(Iterable&& iterable, Args&&... args)
                    -> decltype(hybrid_adapter_impl_base<Sorters...>::detail_stability(
                        choice_for_it<decltype(std::begin(iterable))>{},
                        std::forward<Iterable>(iterable),
                        std::forward<Args>(args)...
                    ))
                {
                    return {};
                }

                template<typename Iterator, typename... Args>
                static auto detail_stability(Iterator first, Iterator last, Args&&... args)
                    -> decltype(hybrid_adapter_impl_base<Sorters...>::detail_stability(
                            choice_for_it<Iterator>{},
                            std::move(first), std::move(last),
                            std::forward<Args>(args)...
                    ))
                {
                    return {};
                }

                ////////////////////////////////////////////////////////////
                // access to the stored sorters

                template<std::size_t I>
                auto get() &
                    -> decltype(auto)
                {
                    using type = std::tuple_element_t<I, std::tuple<Sorters...>>;
                    constexpr auto id = I + iterator_category_value<iterator_category<type>> * categories_number;
                    return static_cast<hybrid_adapter_storage_leaf<id, type>&>(*this).get();
                }

                template<std::size_t I>
                auto get() const&
                    -> decltype(auto)
                {
                    using type = std::tuple_element_t<I, std::tuple<Sorters...>>;
                    constexpr auto id = I + iterator_category_value<iterator_category<type>> * categories_number;
                    return static_cast<const hybrid_adapter_storage_leaf<id, type>&>(*this).get();
                }

                template<std::size_t I>
                auto get() &&
                    -> decltype(auto)
                {
                    using type = std::tuple_element_t<I, std::tuple<Sorters...>>;
                    constexpr auto id = I + iterator_category_value<iterator_category<type>> * categories_number;
                    return static_cast<hybrid_adapter_storage_leaf<id, type>&&>(*this).get();
                }

                template<std::size_t I>
                auto get() const&&
                    -> decltype(auto)
                {
                    using type = std::tuple_element_t<I, std::tuple<Sorters...>>;
                    constexpr auto id = I + iterator_category_value<iterator_category<type>> * categories_number;
                    return static_cast<const hybrid_adapter_storage_leaf<id, type>&&>(*this).get();
                }
        };
    }

    template<typename... Sorters>
    struct hybrid_adapter:
        sorter_facade<detail::hybrid_adapter_impl<Sorters...>>
    {
        hybrid_adapter() = default;

        constexpr hybrid_adapter(Sorters... sorters):
            sorter_facade<detail::hybrid_adapter_impl<Sorters...>>(std::move(sorters)...)
        {};
    };

    ////////////////////////////////////////////////////////////
    // is_stable specializations

    template<typename... Sorters, typename... Args>
    struct is_stable<hybrid_adapter<Sorters...>(Args...)>:
        decltype(hybrid_adapter<Sorters...>::detail_stability(std::declval<Args&>()...))
    {};
}

#endif // CPPSORT_ADAPTERS_HYBRID_ADAPTER_H_
