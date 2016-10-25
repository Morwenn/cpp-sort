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
#include <cpp-sort/utility/logical_traits.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // With default_sorter

    template<typename Iterable>
    auto stable_sort(Iterable&& iterable)
        -> void
    {
        stable_adapter<default_sorter>{}(std::forward<Iterable>(iterable));
    }

    template<
        typename Iterable,
        typename Compare,
        typename = std::enable_if_t<not is_sorter_v<Iterable, Compare>>
    >
    auto stable_sort(Iterable&& iterable, Compare compare)
        -> void
    {
        stable_adapter<default_sorter>{}(std::forward<Iterable>(iterable), std::move(compare));
    }

    template<
        typename Iterable,
        typename Compare,
        typename Projection,
        typename = std::enable_if_t<
            not is_comparison_sorter_v<Iterable, Compare, Projection> &&
            not is_projection_sorter_v<Iterable, Compare, Projection>
        >
    >
    auto stable_sort(Iterable&& iterable, Compare compare, Projection projection)
        -> void
    {
        stable_adapter<default_sorter>{}(std::forward<Iterable>(iterable),
                                         std::move(compare), std::move(projection));
    }

    template<typename Iterator>
    auto stable_sort(Iterator first, Iterator last)
        -> void
    {
        stable_adapter<default_sorter>{}(std::move(first), std::move(last));
    }

    template<
        typename Iterator,
        typename Compare,
        typename = std::enable_if_t<not is_sorter_iterator_v<Compare, Iterator>>
    >
    auto stable_sort(Iterator first, Iterator last, Compare compare)
        -> void
    {
        stable_adapter<default_sorter>{}(std::move(first), std::move(last),
                                         std::move(compare));
    }

    template<
        typename Iterator,
        typename Compare,
        typename Projection,
        typename = std::enable_if_t<
            not is_comparison_sorter_iterator_v<Compare, Iterator, Projection> &&
            not is_projection_sorter_iterator_v<Compare, Iterator, Projection>
        >
    >
    auto stable_sort(Iterator first, Iterator last, Compare compare, Projection projection)
        -> void
    {
        return stable_adapter<default_sorter>{}(std::move(first), std::move(last),
                                                std::move(compare), std::move(projection));
    }

    ////////////////////////////////////////////////////////////
    // With a given sorter

    template<
        typename Sorter,
        typename Iterable,
        typename = std::enable_if_t<
            is_sorter_v<Sorter, Iterable>
        >
    >
    auto stable_sort(const Sorter&, Iterable&& iterable)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::forward<Iterable>(iterable));
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Func,
        typename = std::enable_if_t<utility::disjunction<
            is_comparison_sorter<Sorter, Iterable, Func>,
            is_projection_sorter<Sorter, Iterable, Func>
        >::value>
    >
    auto stable_sort(const Sorter&, Iterable&& iterable, Func func)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::forward<Iterable>(iterable), std::move(func));
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Compare,
        typename Projection
    >
    auto stable_sort(const Sorter&, Iterable&& iterable,
                     Compare compare, Projection projection)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::forward<Iterable>(iterable),
                                        std::move(compare), std::move(projection));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename = std::enable_if_t<is_sorter_iterator_v<
            stable_adapter<Sorter>, Iterator
        >>
    >
    auto stable_sort(const Sorter&, Iterator first, Iterator last)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::move(first), std::move(last));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Func,
        typename = std::enable_if_t<
            is_comparison_sorter_iterator_v<stable_adapter<Sorter>, Iterator, Func> ||
            is_projection_sorter_iterator_v<stable_adapter<Sorter>, Iterator, Func>
        >
    >
    auto stable_sort(const Sorter&, Iterator first, Iterator last, Func func)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::move(first), std::move(last), std::move(func));
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Compare,
        typename Projection
    >
    auto stable_sort(const Sorter&, Iterator first, Iterator last,
                     Compare compare, Projection projection)
        -> decltype(auto)
    {
        return stable_adapter<Sorter>{}(std::move(first), std::move(last),
                                        std::move(compare), std::move(projection));
    }
}

#endif // CPPSORT_STABLE_SORT_H_
