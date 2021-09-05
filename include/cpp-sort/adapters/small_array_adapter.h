/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
#define CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/any_all.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    namespace detail
    {
        ////////////////////////////////////////////////////////////
        // SFINAE helpers

        template<typename T, typename=void>
        struct has_domain:
            std::false_type
        {
            using domain = void;
        };

        template<typename T>
        struct has_domain<T, void_t<typename T::domain>>:
            std::true_type
        {
            using domain = typename T::domain;
        };
    }

    ////////////////////////////////////////////////////////////
    // Adapter

    // When no domain is given along with the fixed-size sorter,
    // it is assumed that it works for small arrays of any size
    // and thus void is given

    template<
        template<std::size_t> class FixedSizeSorter,
        typename Indices = typename detail::has_domain<
            fixed_sorter_traits<FixedSizeSorter>
        >::domain
    >
    struct small_array_adapter;

    template<
        template<std::size_t> class FixedSizeSorter,
        std::size_t... Indices
    >
    struct small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>:
        fixed_sorter_traits<FixedSizeSorter>,
        detail::sorter_facade_fptr<
            small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>,
            detail::all(std::is_empty<FixedSizeSorter<Indices>>::value...)
        >
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> detail::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(FixedSizeSorter<N>{}(array, std::forward<Args>(args)...))
            >
        {
            return FixedSizeSorter<N>{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args,
            typename = detail::enable_if_t<detail::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> detail::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(FixedSizeSorter<N>{}(array, std::forward<Args>(args)...))
            >
        {
            return FixedSizeSorter<N>{}(array, std::forward<Args>(args)...);
        }
    };

    template<template<std::size_t> class FixedSizeSorter>
    struct small_array_adapter<FixedSizeSorter, void>:
        fixed_sorter_traits<FixedSizeSorter>,
        detail::sorter_facade_fptr<
            small_array_adapter<FixedSizeSorter, void>,
            true // TODO: how can we do better?
        >
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> decltype(FixedSizeSorter<N>{}(array, std::forward<Args>(args)...))
        {
            return FixedSizeSorter<N>{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> decltype(FixedSizeSorter<N>{}(array, std::forward<Args>(args)...))
        {
            return FixedSizeSorter<N>{}(array, std::forward<Args>(args)...);
        }
    };

    ////////////////////////////////////////////////////////////
    // is_stable specialization

    template<
        template<std::size_t> class FixedSizeSorter,
        typename Indices,
        typename T, std::size_t N,
        typename... Args
    >
    struct is_stable<small_array_adapter<FixedSizeSorter, Indices>(std::array<T, N>&, Args...)>:
        is_stable<FixedSizeSorter<N>(std::array<T, N>&, Args...)>
    {};

    template<
        template<std::size_t> class FixedSizeSorter,
        typename Indices,
        typename T, std::size_t N,
        typename... Args
    >
    struct is_stable<small_array_adapter<FixedSizeSorter, Indices>(T (&)[N], Args...)>:
        is_stable<FixedSizeSorter<N>(T (&)[N], Args...)>
    {};
}

#ifdef CPPSORT_ADAPTERS_SCHWARTZ_ADAPTER_H_
#include "../detail/schwartz_small_array.h"
#endif

#endif // CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
