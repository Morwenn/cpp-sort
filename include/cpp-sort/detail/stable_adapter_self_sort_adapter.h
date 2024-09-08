/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_STABLE_ADAPTER_SELF_SORT_ADAPTER_H_
#define CPPSORT_DETAIL_STABLE_ADAPTER_SELF_SORT_ADAPTER_H_

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
#include "checkers.h"

namespace cppsort
{
    template<typename Sorter>
    struct stable_adapter<self_sort_adapter<Sorter>>:
        utility::adapter_storage<stable_t<Sorter>>,
        detail::check_iterator_category<Sorter>,
        detail::sorter_facade_fptr<
            stable_adapter<self_sort_adapter<Sorter>>,
            std::is_empty<Sorter>::value
        >
    {
        ////////////////////////////////////////////////////////////
        // Construction

        stable_adapter() = default;

        constexpr explicit stable_adapter(self_sort_adapter<Sorter> sorter):
            utility::adapter_storage<stable_t<Sorter>>(
                stable_t<Sorter>(std::move(sorter.get()))
            )
        {}

        ////////////////////////////////////////////////////////////
        // Generic cases

        template<typename Collection, typename... Args>
            requires detail::has_stable_sort_method<Collection, Args...>
        static auto operator()(Collection&& collection, Args&&... args)
            -> decltype(std::forward<Collection>(collection).stable_sort(utility::as_function(args)...))
        {
            return std::forward<Collection>(collection).stable_sort(utility::as_function(args)...);
        }

        template<typename Self, typename Collection, typename... Args>
            requires (not detail::has_stable_sort_method<Collection, Args...>)
        auto operator()(this Self&& self, Collection&& collection, Args&&... args)
            -> decltype(std::forward<Self>(self).get()(
                std::forward<Collection>(collection), std::forward<Args>(args)...))
        {
            return std::forward<Self>(self).get()(
                std::forward<Collection>(collection), std::forward<Args>(args)...);
        }

        template<typename Self, typename Iterator, typename... Args>
        auto operator()(this Self&& self, Iterator first, Iterator last, Args&&... args)
            -> decltype(std::forward<Self>(self).get()(first, last, std::forward<Args>(args)...))
        {
            return std::forward<Self>(self).get()(first, last, std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Special cases for standard library lists whose sort
        // method implements a stable sort

        template<typename T>
        static auto operator()(std::forward_list<T>& collection)
            -> void
        {
            collection.sort();
        }

        template<typename T, typename Compare>
            requires (not is_projection_v<Compare, std::forward_list<T>&>)
        static auto operator()(std::forward_list<T>& collection, Compare compare)
            -> void
        {
            collection.sort(utility::as_function(compare));
        }

        template<typename T>
        static auto operator()(std::list<T>& collection)
            -> void
        {
            collection.sort();
        }

        template<typename T, typename Compare>
            requires (not is_projection_v<Compare, std::list<T>&>)
        static auto operator()(std::list<T>& collection, Compare compare)
            -> void
        {
            collection.sort(utility::as_function(compare));
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using is_always_stable = std::true_type;
    };
}

#endif // CPPSORT_DETAIL_STABLE_ADAPTER_SELF_SORT_ADAPTER_H_
