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
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/size.h>

template<
    typename ForwardIterator,
    typename StrictWeakOrdering
>
auto bubble_sort(ForwardIterator first, StrictWeakOrdering compare,
                 std::size_t size)
    -> void
{
    if (size < 2) return;

    while (--size)
    {
        ForwardIterator current = first;
        ForwardIterator next = std::next(current);
        for (std::size_t i = 0 ; i < size ; ++i)
        {
            if (compare(*next, *current))
            {
                std::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
    }
}

struct bubble_sorter:
    cppsort::sorter_facade<bubble_sorter>
{
    // Pair of iterators overload
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering = std::less<>
    >
    auto operator()(ForwardIterator first, ForwardIterator last,
                    StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(first, compare, std::distance(first, last));
    }

    // Iterable overload
    template<
        typename ForwardIterable,
        typename StrictWeakOrdering = std::less<>
    >
    auto operator()(ForwardIterable& iterable, StrictWeakOrdering compare={}) const
        -> void
    {
        bubble_sort(
            std::begin(iterable), compare,
            cppsort::utility::size(iterable)
        );
    }
};

namespace cppsort
{
    template<>
    struct sorter_traits<::bubble_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = true;
    };
}

#include <array>
#include <cassert>
#include <numeric>

int main()
{
    // Check that the bubble_sorter can sort any permutation
    // of an array of 8 values

    // Fill the collection in sorted order
    std::array<int, 8> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // For each possible permutation of collection
    do
    {
        auto to_sort = collection;
        // Bubble sort the collection
        bubble_sorter{}(to_sort);
        // Check that it is indeed sorted
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort)));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));
}
