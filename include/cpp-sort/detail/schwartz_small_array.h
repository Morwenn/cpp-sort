/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#ifndef CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_
#define CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/is_in_pack.h"

namespace cppsort
{
    template<
        template<std::size_t> class FixedSizeSorter,
        size_t... Indices
    >
    struct schwartz_adapter<
        small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>
    >:
        fixed_sorter_traits<FixedSizeSorter>,
        sorter_facade_fptr<schwartz_adapter<small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>>>>
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> std::enable_if_t<
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
            typename = std::enable_if_t<detail::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> std::enable_if_t<
                detail::is_in_pack<N, Indices...>,
                decltype(schwartz_adapter<FixedSizeSorter<N>>{}(array, std::forward<Args>(args)...))
            >
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }
    };

    template<template<std::size_t> class FixedSizeSorter>
    struct schwartz_adapter<small_array_adapter<FixedSizeSorter, void>>:
        fixed_sorter_traits<FixedSizeSorter>,
        sorter_facade_fptr<schwartz_adapter<small_array_adapter<FixedSizeSorter, void>>>
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
