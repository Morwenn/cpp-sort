/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_STABLE_SORT_H_
#define CPPSORT_STABLE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <utility>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/default_sorter.h>
#include "detail/config.h"
#include "detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // With default_sorter

    template<typename Iterable>
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterable&& iterable)
        -> void
    {
        stable_t<default_sorter>{}(std::forward<Iterable>(iterable));
    }

    template<
        typename Iterable,
        typename Compare,
        typename = detail::enable_if_t<not is_sorter_v<Iterable, Compare>>
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterable&& iterable, Compare compare)
        -> void
    {
        stable_t<default_sorter>{}(std::forward<Iterable>(iterable), std::move(compare));
    }

    template<
        typename Iterable,
        typename Compare,
        typename Projection,
        typename = detail::enable_if_t<
            not is_comparison_sorter_v<Iterable, Compare, Projection> &&
            not is_projection_sorter_v<Iterable, Compare, Projection>
        >
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterable&& iterable, Compare compare, Projection projection)
        -> void
    {
        stable_t<default_sorter>{}(std::forward<Iterable>(iterable),
                                   std::move(compare), std::move(projection));
    }

    template<typename Iterator>
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterator first, Iterator last)
        -> void
    {
        stable_t<default_sorter>{}(std::move(first), std::move(last));
    }

    template<
        typename Iterator,
        typename Compare,
        typename = detail::enable_if_t<not is_sorter_iterator_v<Compare, Iterator>>
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterator first, Iterator last, Compare compare)
        -> void
    {
        stable_t<default_sorter>{}(std::move(first), std::move(last), std::move(compare));
    }

    template<
        typename Iterator,
        typename Compare,
        typename Projection,
        typename = detail::enable_if_t<
            not is_comparison_sorter_iterator_v<Compare, Iterator, Projection> &&
            not is_projection_sorter_iterator_v<Compare, Iterator, Projection>
        >
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(Iterator first, Iterator last, Compare compare, Projection projection)
        -> void
    {
        return stable_t<default_sorter>{}(std::move(first), std::move(last),
                                          std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // With a given sorter

    template<
        typename Sorter,
        typename Iterable,
        typename = detail::enable_if_t<
            is_sorter_v<Sorter, Iterable>
        >
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterable&& iterable)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::forward<Iterable>(iterable));
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Func,
        typename = detail::enable_if_t<detail::disjunction<
            is_comparison_sorter<Sorter, Iterable, Func>,
            is_projection_sorter<Sorter, Iterable, Func>
        >::value>
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterable&& iterable, Func func)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::forward<Iterable>(iterable), std::move(func));
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Compare,
        typename Projection
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterable&& iterable,
                     Compare compare, Projection projection)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::forward<Iterable>(iterable),
                                  std::move(compare), std::move(projection));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename = detail::enable_if_t<is_sorter_iterator_v<
            stable_t<Sorter>, Iterator
        >>
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterator first, Iterator last)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::move(first), std::move(last));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Func,
        typename = detail::enable_if_t<
            is_comparison_sorter_iterator_v<stable_t<Sorter>, Iterator, Func> ||
            is_projection_sorter_iterator_v<stable_t<Sorter>, Iterator, Func>
        >
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterator first, Iterator last, Func func)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::move(first), std::move(last), std::move(func));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Compare,
        typename Projection
    >
    CPPSORT_DEPRECATED("cppsort::stable_sort() is deprecated and will be removed in version 2.0.0")
    auto stable_sort(const Sorter&, Iterator first, Iterator last,
                     Compare compare, Projection projection)
        -> decltype(auto)
    {
        return stable_t<Sorter>{}(std::move(first), std::move(last),
                                  std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_STABLE_SORT_H_
