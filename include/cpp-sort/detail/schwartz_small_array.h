/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_
#define CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/type_traits.h"

namespace cppsort
{
    template<
        template<std::size_t> typename FixedSizeSorter,
        size_t... Indices
    >
    struct schwartz_adapter<
        small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>
    >:
        fixed_sorter_traits<FixedSizeSorter>,
        detail::sorter_facade_fptr<
            schwartz_adapter<small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>>,
            (std::is_empty_v<FixedSizeSorter<Indices>> && ...)
        >
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> mstd::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(schwartz_adapter<FixedSizeSorter<N>>{}(array, std::forward<Args>(args)...))
            >
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args,
            typename = mstd::enable_if_t<detail::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> mstd::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(schwartz_adapter<FixedSizeSorter<N>>{}(array, std::forward<Args>(args)...))
            >
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }
    };

    template<template<std::size_t> typename FixedSizeSorter>
    struct schwartz_adapter<small_array_adapter<FixedSizeSorter, void>>:
        fixed_sorter_traits<FixedSizeSorter>,
        detail::sorter_facade_fptr<
            schwartz_adapter<small_array_adapter<FixedSizeSorter, void>>,
            true // TODO: not sure how to specify that one
        >
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> decltype(schwartz_adapter<FixedSizeSorter<N>>{}(array, std::forward<Args>(args)...))
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> decltype(schwartz_adapter<FixedSizeSorter<N>>{}(array, std::forward<Args>(args)...))
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }
    };
}

#endif // CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_
