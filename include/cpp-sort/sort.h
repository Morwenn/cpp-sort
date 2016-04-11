/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#ifndef CPPSORT_SORT_H_
#define CPPSORT_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/default_sorter.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // With default_sorter

    template<typename Iterable>
    auto sort(Iterable& iterable)
        -> void
    {
        default_sorter{}(iterable);
    }

    template<
        typename Iterable,
        typename Compare,
        typename = std::enable_if_t<not is_sorter_v<Iterable, Compare>>
    >
    auto sort(Iterable& iterable, Compare compare)
        -> void
    {
        default_sorter{}(iterable, compare);
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
    auto sort(Iterable& iterable, Compare compare, Projection projection)
        -> void
    {
        default_sorter{}(iterable, compare, projection);
    }

    template<typename Iterator>
    auto sort(Iterator first, Iterator last)
        -> void
    {
        default_sorter{}(first, last);
    }

    template<
        typename Iterator,
        typename Compare,
        typename = std::enable_if_t<not is_sorter_iterator_v<Iterator, Compare>>
    >
    auto sort(Iterator first, Iterator last, Compare compare)
        -> void
    {
        default_sorter{}(first, last, compare);
    }

    template<
        typename Iterator,
        typename Compare,
        typename Projection,
        typename = std::enable_if_t<
            not is_comparison_sorter_iterator_v<Iterator, Compare, Projection> &&
            not is_projection_sorter_iterator_v<Iterator, Compare, Projection>
        >
    >
    auto sort(Iterator first, Iterator last, Compare compare, Projection projection)
        -> void
    {
        return default_sorter{}(first, last, compare, projection);
    }

    ////////////////////////////////////////////////////////////
    // With a given sorter

    template<
        typename Sorter,
        typename Iterable,
        typename = std::enable_if_t<is_sorter_v<Sorter, Iterable>>
    >
    auto sort(const Sorter& sorter, Iterable& iterable)
        -> decltype(auto)
    {
        return sorter(iterable);
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Func,
        typename = std::enable_if_t<
            is_comparison_sorter_v<Sorter, Iterable, Func> ||
            is_projection_sorter_v<Sorter, Iterable, Func>
        >
    >
    auto sort(const Sorter& sorter, Iterable& iterable, Func func)
        -> decltype(auto)
    {
        return sorter(iterable, func);
    }

    template<
        typename Sorter,
        typename Iterable,
        typename Compare,
        typename Projection
    >
    auto sort(const Sorter& sorter, Iterable& iterable,
              Compare compare, Projection projection)
        -> decltype(auto)
    {
        return sorter(iterable, compare, projection);
    }

    template<
        typename Sorter,
        typename Iterator,
        typename = std::enable_if_t<is_sorter_iterator_v<Sorter, Iterator>>
    >
    auto sort(const Sorter& sorter, Iterator first, Iterator last)
        -> decltype(auto)
    {
        return sorter(first, last);
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Func,
        typename = std::enable_if_t<
            is_comparison_sorter_iterator_v<Sorter, Iterator, Func> ||
            is_projection_sorter_iterator_v<Sorter, Iterator, Func>
        >
    >
    auto sort(const Sorter& sorter, Iterator first, Iterator last, Func func)
        -> decltype(auto)
    {
        return sorter(first, last, func);
    }

    template<
        typename Sorter,
        typename Iterator,
        typename Compare,
        typename Projection
    >
    auto sort(const Sorter& sorter, Iterator first, Iterator last,
              Compare compare, Projection projection)
        -> decltype(auto)
    {
        return sorter(first, last, compare, projection);
    }
}

#endif // CPPSORT_SORT_H_
