/*
 * Copyright (c) 2016-2021 Morwenn
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
#include "type_traits.h"

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

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> detail::enable_if_t<
                detail::has_stable_sort_method<Iterable, Args...>,
                decltype(std::forward<Iterable>(iterable).stable_sort(utility::as_function(args)...))
            >
        {
            return std::forward<Iterable>(iterable).stable_sort(utility::as_function(args)...);
        }

        template<typename Iterable, typename... Args>
        auto operator()(Iterable&& iterable, Args&&... args) const
            -> detail::enable_if_t<
                not detail::has_stable_sort_method<Iterable, Args...>,
                decltype(this->get()(std::forward<Iterable>(iterable), std::forward<Args>(args)...))
            >
        {
            return this->get()(std::forward<Iterable>(iterable), std::forward<Args>(args)...);
        }

        template<typename Iterator, typename... Args>
        auto operator()(Iterator first, Iterator last, Args&&... args) const
            -> decltype(this->get()(first, last, std::forward<Args>(args)...))
        {
            return this->get()(first, last, std::forward<Args>(args)...);
        }

        ////////////////////////////////////////////////////////////
        // Special cases for standard library lists whose sort
        // method implements a stable sort

        template<typename T>
        auto operator()(std::forward_list<T>& iterable) const
            -> void
        {
            iterable.sort();
        }

        template<
            typename T,
            typename Compare,
            typename = detail::enable_if_t<not is_projection_v<Compare, std::forward_list<T>&>>
        >
        auto operator()(std::forward_list<T>& iterable, Compare compare) const
            -> void
        {
            iterable.sort(utility::as_function(compare));
        }

        template<typename T>
        auto operator()(std::list<T>& iterable) const
            -> void
        {
            iterable.sort();
        }

        template<
            typename T,
            typename Compare,
            typename = detail::enable_if_t<not is_projection_v<Compare, std::list<T>&>>
        >
        auto operator()(std::list<T>& iterable, Compare compare) const
            -> void
        {
            iterable.sort(utility::as_function(compare));
        }

        ////////////////////////////////////////////////////////////
        // Sorter traits

        using is_always_stable = std::true_type;
    };
}

#endif // CPPSORT_DETAIL_STABLE_ADAPTER_SELF_SORT_ADAPTER_H_
