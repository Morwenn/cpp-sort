/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
#define CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <forward_list>
#include <list>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/checkers.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Whether a class has a member sort method

        template<typename Collection, typename... Args>
        concept has_sort_method =
            requires (Collection&& collection, Args&&... args) {
                std::forward<Collection>(collection).sort(std::forward<Args>(args)...);
            };

        template<typename Collection, typename... Args>
        concept has_stable_sort_method =
            requires (Collection&& collection, Args&&... args) {
                std::forward<Collection>(collection).stable_sort(std::forward<Args>(args)...);
            };
    }

    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename Sorter>
    struct self_sort_adapter:
        utility::adapter_storage<Sorter>,
        detail::check_iterator_category<Sorter>,
        detail::sorter_facade_fptr<
            self_sort_adapter<Sorter>,
            std::is_empty_v<Sorter>
        >
    {
        ////////////////////////////////////////////////////////////
        // Construction

        self_sort_adapter() = default;

        constexpr explicit self_sort_adapter(Sorter sorter):
            utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        ////////////////////////////////////////////////////////////
        // Function call operator

        template<typename Collection, typename... Args>
            requires detail::has_sort_method<Collection, Args...>
        static auto operator()(Collection&& collection, Args&&... args)
            -> decltype(std::forward<Collection>(collection).sort(utility::as_function(args)...))
        {
            return std::forward<Collection>(collection).sort(utility::as_function(args)...);
        }

        template<typename Collection, typename... Args>
            requires (
                not detail::has_sort_method<Collection, Args...> &&
                detail::has_stable_sort_method<Collection, Args...>
            )
        static auto operator()(Collection&& collection, Args&&... args)
            -> decltype(std::forward<Collection>(collection).stable_sort(utility::as_function(args)...))
        {
            return std::forward<Collection>(collection).stable_sort(utility::as_function(args)...);
        }

        template<typename Self, typename Collection, typename... Args>
            requires (
                not detail::has_sort_method<Collection, Args...> &&
                not detail::has_stable_sort_method<Collection, Args...>
            )
        auto operator()(this Self&& self, Collection&& collection, Args&&... args)
            -> decltype(std::forward<Self>(self).get()(
                std::forward<Collection>(collection), std::forward<Args>(args)...))
        {
            return std::forward<Self>(self).get()(
                std::forward<Collection>(collection), std::forward<Args>(args)...);
        }

        template<typename Self, typename Iterator, typename... Args>
        auto operator()(this Self&& self, Iterator first, Iterator last, Args&&... args)
            -> decltype(std::forward<Self>(self).get()(
                std::move(first), std::move(last), std::forward<Args>(args)...))
        {
            return std::forward<Self>(self).get()(
                std::move(first), std::move(last), std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using is_always_stable = std::false_type;
    };

    ////////////////////////////////////////////////////////////
    // is_stable specializations

    template<typename Sorter, typename Iterator, typename... Args>
    struct is_stable<self_sort_adapter<Sorter>(Iterator, Iterator, Args...)>:
        is_stable<Sorter(Iterator, Iterator, Args...)>
    {};

    template<typename Sorter, typename Collection, typename... Args>
    struct is_stable<self_sort_adapter<Sorter>(Collection, Args...)>:
        mstd::conditional_t<
            detail::has_sort_method<Collection, Args...>,
            std::false_type,
            mstd::conditional_t<
                detail::has_stable_sort_method<Collection, Args...>,
                std::true_type,
                is_stable<Sorter(Collection, Args...)>
            >
        >
    {};

    template<typename Sorter, typename T>
    struct is_stable<self_sort_adapter<Sorter>(std::forward_list<T>&)>:
        std::true_type
    {};

    template<typename Sorter, typename T, typename Function>
    struct is_stable<self_sort_adapter<Sorter>(std::forward_list<T>&, Function)>:
        mstd::conditional_t<
            is_projection_v<Function, std::forward_list<T>&>,
            is_stable<Sorter(std::forward_list<T>&, Function)>,
            std::true_type
        >
    {};

    template<typename Sorter, typename T>
    struct is_stable<self_sort_adapter<Sorter>(std::list<T>&)>:
        std::true_type
    {};

    template<typename Sorter, typename T, typename Function>
    struct is_stable<self_sort_adapter<Sorter>(std::list<T>&, Function)>:
        mstd::conditional_t<
            is_projection_v<Function, std::list<T>&>,
            is_stable<Sorter(std::list<T>&, Function)>,
            std::true_type
        >
    {};
}

#ifdef CPPSORT_ADAPTERS_STABLE_ADAPTER_DONE_
#include "../detail/stable_adapter_self_sort_adapter.h"
#endif

#define CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_DONE_

#endif // CPPSORT_ADAPTERS_SELF_SORT_ADAPTER_H_
