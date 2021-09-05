/*
 * Copyright (c) 2015-2021 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/checkers.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // Whether a class has a member sort method

        template<
            typename Iterable,
            typename... Args
        >
        using has_sort_method_t
            = decltype(std::declval<Iterable&>().sort(utility::as_function(std::declval<Args&>())...));

        template<
            typename Iterable,
            typename... Args
        >
        constexpr bool has_sort_method
            = is_detected_v<has_sort_method_t, Iterable, Args...>;

        template<
            typename Iterable,
            typename... Args
        >
        using has_stable_sort_method_t
            = decltype(std::declval<Iterable&>().stable_sort(utility::as_function(std::declval<Args&>())...));

        template<
            typename Iterable,
            typename... Args
        >
        constexpr bool has_stable_sort_method
            = is_detected_v<has_stable_sort_method_t, Iterable, Args...>;
    }

    ////////////////////////////////////////////////////////////
    // Adapter

    template<typename Sorter>
    struct self_sort_adapter:
        utility::adapter_storage<Sorter>,
        detail::check_iterator_category<Sorter>,
        detail::sorter_facade_fptr<
            self_sort_adapter<Sorter>,
            std::is_empty<Sorter>::value
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

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> detail::enable_if_t<
                detail::has_sort_method<Iterable, Args...>,
                decltype(std::forward<Iterable>(iterable).sort(utility::as_function(args)...))
            >
        {
            return std::forward<Iterable>(iterable).sort(utility::as_function(args)...);
        }

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> detail::enable_if_t<
                not detail::has_sort_method<Iterable, Args...> &&
                detail::has_stable_sort_method<Iterable, Args...>,
                decltype(std::forward<Iterable>(iterable).stable_sort(utility::as_function(args)...))
            >
        {
            return std::forward<Iterable>(iterable).stable_sort(utility::as_function(args)...);
        }

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> detail::enable_if_t<
                not detail::has_sort_method<Iterable, Args...> &&
                not detail::has_stable_sort_method<Iterable, Args...>,
                decltype(this->get()(std::forward<Iterable>(iterable), std::forward<Args>(args)...))
            >
        {
            return this->get()(std::forward<Iterable>(iterable), std::forward<Args>(args)...);
        }

        template<typename Iterator, typename... Args>
        auto operator()(Iterator first, Iterator last, Args&&... args) const
            -> decltype(this->get()(std::move(first), std::move(last),
                                    std::forward<Args>(args)...))
        {
            return this->get()(std::move(first), std::move(last),
                               std::forward<Args>(args)...);
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

    template<typename Sorter, typename... Args>
    struct is_stable<self_sort_adapter<Sorter>(Args...)>:
        detail::conditional_t<
            detail::has_sort_method<Args...>,
            std::false_type,
            detail::conditional_t<
                detail::has_stable_sort_method<Args...>,
                std::true_type,
                is_stable<Sorter(Args...)>
            >
        >
    {};

    template<typename Sorter, typename T>
    struct is_stable<self_sort_adapter<Sorter>(std::forward_list<T>&)>:
        std::true_type
    {};

    template<typename Sorter, typename T, typename Function>
    struct is_stable<self_sort_adapter<Sorter>(std::forward_list<T>&, Function)>:
        detail::conditional_t<
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
        detail::conditional_t<
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
