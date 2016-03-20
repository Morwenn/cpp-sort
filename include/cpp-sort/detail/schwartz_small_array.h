/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <functional>
#include <iterator>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/is_in_pack.h>
#include "../detail/checkers.h"

namespace cppsort
{
    template<
        template<std::size_t, typename...> class FixedSizeSorter,
        size_t... Indices,
        typename... Options
    >
    struct schwartz_adapter<
        small_array_adapter<FixedSizeSorter, std::index_sequence<Indices...>, Options...>
    >
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> std::enable_if_t<
                utility::is_in_pack<N, Indices...>,
                decltype(schwartz_adapter<FixedSizeSorter<N, Options...>>{}(array, std::forward<Args>(args)...))
            >
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N, Options...>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args,
            typename = std::enable_if_t<utility::is_in_pack<N, Indices...>>
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> std::enable_if_t<
                utility::is_in_pack<N, Indices...>,
                decltype(schwartz_adapter<FixedSizeSorter<N, Options...>>{}(array, std::forward<Args>(args)...))
            >
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N, Options...>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }
    };

    template<
        template<std::size_t, typename...> class FixedSizeSorter,
        typename... Options
    >
    struct schwartz_adapter<small_array_adapter<FixedSizeSorter, Options...>>
    {
        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(std::array<T, N>& array, Args&&... args) const
            -> decltype(schwartz_adapter<FixedSizeSorter<N, Options...>>{}(array, std::forward<Args>(args)...))
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N, Options...>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }

        template<
            typename T,
            std::size_t N,
            typename... Args
        >
        auto operator()(T (&array)[N], Args&&... args) const
            -> decltype(schwartz_adapter<FixedSizeSorter<N, Options...>>{}(array, std::forward<Args>(args)...))
        {
            using sorter = schwartz_adapter<FixedSizeSorter<N, Options...>>;
            return sorter{}(array, std::forward<Args>(args)...);
        }
    };
}

#endif // CPPSORT_ADAPTERS_SCHWARTZ_SMALL_ARRAY_H_
