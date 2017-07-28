/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#ifndef CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_
#define CPPSORT_ADAPTERS_SMALL_ARRAY_ADAPTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include "../detail/detection.h"
#include "../detail/is_in_pack.h"

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
        fixed_sorter_traits<FixedSizeSorter>
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> std::enable_if_t<
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
            typename = std::enable_if_t<detail::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> std::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(FixedSizeSorter<N>{}(array, std::forward<Args>(args)...))
            >
        {
            return FixedSizeSorter<N>{}(array, std::forward<Args>(args)...);
        }
    };

    template<template<std::size_t> class FixedSizeSorter>
    struct small_array_adapter<FixedSizeSorter, void>:
        fixed_sorter_traits<FixedSizeSorter>
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
