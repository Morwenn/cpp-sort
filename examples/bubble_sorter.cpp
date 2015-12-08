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
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>

namespace detail
{
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering,
        typename Projection
    >
    auto bubble_sort(ForwardIterator first, StrictWeakOrdering compare,
                     Projection projection, std::size_t size)
        -> void
    {
        if (size < 2) return;

        auto&& proj = cppsort::utility::as_function(projection);

        while (--size)
        {
            ForwardIterator current = first;
            ForwardIterator next = std::next(current);
            for (std::size_t i = 0 ; i < size ; ++i)
            {
                if (compare(proj(*next), proj(*current)))
                {
                    std::iter_swap(current, next);
                }
                ++next;
                ++current;
            }
        }
    }

    struct bubble_sorter_impl
    {
        // Pair of iterators overload
        template<
            typename ForwardIterator,
            typename StrictWeakOrdering = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator<
                Projection, ForwardIterator, StrictWeakOrdering
            >>
        >
        auto operator()(ForwardIterator first, ForwardIterator last,
                        StrictWeakOrdering compare={}, Projection projection={}) const
            -> void
        {
            static_assert(
                std::is_base_of<
                    std::forward_iterator_tag,
                    typename std::iterator_traits<ForwardIterator>::iterator_category
                >::value,
                "bubble_sorter requires at least forward iterators"
            );

            bubble_sort(first,
                        compare, projection,
                        std::distance(first, last));
        }

        // Iterable overload
        template<
            typename ForwardIterable,
            typename StrictWeakOrdering = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection<
                Projection, ForwardIterable, StrictWeakOrdering
            >>
        >
        auto operator()(ForwardIterable& iterable, StrictWeakOrdering compare={},
                        Projection projection={}) const
            -> void
        {
            static_assert(
                std::is_base_of<
                    std::forward_iterator_tag,
                    typename std::iterator_traits<decltype(std::begin(iterable))>::iterator_category
                >::value,
                "bubble_sorter requires at least forward iterators"
            );

            bubble_sort(std::begin(iterable),
                        compare, projection,
                        cppsort::utility::size(iterable));
        }
    };
}

struct bubble_sorter:
    cppsort::sorter_facade<detail::bubble_sorter_impl>
{};

namespace cppsort
{
    template<>
    struct sorter_traits<::bubble_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        using is_stable = std::true_type;
    };
}

#include <array>
#include <cassert>
#include <numeric>
#include <cpp-sort/sort.h>

int main()
{
    // Check that the bubble_sorter can sort any permutation
    // of an array of 8 values

    // Fill the collection in sorted order
    std::array<int, 8> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // Projection to sort in descending order
    auto projection = [](int n) { return -n; };

    // For each possible permutation of collection
    do
    {
        auto to_sort = collection;
        // Bubble sort the collection
        cppsort::sort(to_sort, bubble_sorter{}, projection);
        // Check that it is indeed sorted
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort), std::greater<>{}));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));
}
