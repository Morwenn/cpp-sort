/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#ifndef CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_
#define CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/is_in_pack.h>
#include "../detail/sort_n.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    template<typename...>
    struct small_array_sorter;

    template<
        typename Sorter,
        std::size_t... Indices
    >
    struct small_array_sorter<Sorter, std::index_sequence<Indices...>>
    {
        template<
            typename RandomAccessIterable,
            typename Compare = std::less<>
        >
        auto operator()(RandomAccessIterable& iterable, Compare compare={}) const
            -> void
        {
            Sorter{}(iterable, compare);
        }

        template<
            typename T,
            std::size_t N,
            typename Compare = std::less<>,
            typename = std::enable_if_t<utility::is_in_pack<N, Indices...>>
        >
        auto operator()(std::array<T, N>& array, Compare compare={}) const
            -> void
        {
            detail::sort_n<N, Sorter>(array, compare);
        }

        template<
            typename T,
            std::size_t N,
            typename Compare = std::less<>,
            typename = std::enable_if_t<utility::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Compare compare={}) const
            -> void
        {
            detail::sort_n<N, Sorter>(array, compare);
        }
    };

    template<typename Sorter>
    struct small_array_sorter<Sorter>:
        small_array_sorter<Sorter, std::make_index_sequence<33u>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<typename Sorter, std::size_t... Indices>
    struct sorter_traits<small_array_sorter<Sorter, std::index_sequence<Indices...>>>
    {
        using iterator_category = iterator_category<Sorter>;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        static constexpr bool is_stable = false;
    };
}

#endif // CPPSORT_SORTERS_SMALL_ARRAY_SORTER_H_
