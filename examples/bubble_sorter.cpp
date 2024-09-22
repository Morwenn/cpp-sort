/*
 * Copyright (c) 2015-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace mstd = cppsort::mstd;

namespace detail
{
    template<mstd::forward_iterator Iterator, typename Compare>
    auto bubble_sort_loop(Iterator first, std::size_t size, Compare compare)
        -> Iterator
    {
        auto current = first;
        auto next = mstd::next(current);
        for (std::size_t i = 0; i < size; ++i) {
            if (std::invoke(compare, *next, *current)) {
                mstd::iter_swap(current, next);
            }
            ++next;
            ++current;
        }
        return next;
    }

    template<mstd::forward_iterator Iterator, typename Compare>
    auto bubble_sort(Iterator first, std::size_t size, Compare compare)
        -> Iterator
    {
        if (size == 0) return first;
        if (size == 1) return mstd::next(first);

        // Last iterator can be computed during the first iteration
        auto last_it = bubble_sort_loop(first, --size, compare);

        while (--size) {
            bubble_sort_loop(first, size, compare);
        }
        return last_it;
    }

    struct bubble_sorter_impl
    {
        // Pair of iterators overload
        template<
            mstd::forward_iterator Iterator,
            typename Compare = std::less<>
        >
            requires (not cppsort::is_projection_iterator_v<Compare, Iterator>)
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> Iterator
        {
            return bubble_sort(first, mstd::distance(first, last),
                               std::move(compare));
        }

        // Range overload
        template<
            mstd::forward_range Range,
            typename Compare = std::less<>
        >
            requires (not cppsort::is_projection_v<Compare, Range>)
        auto operator()(Range&& range, Compare compare={}) const
            -> mstd::iterator_t<Range>
        {
            return bubble_sort(mstd::begin(range), mstd::distance(range),
                               std::move(compare));
        }

        // Sorter traits
        using iterator_category = std::forward_iterator_tag;
        using is_always_stable = std::true_type;
    };
}

struct bubble_sorter:
    cppsort::sorter_facade<detail::bubble_sorter_impl>
{};

inline constexpr bubble_sorter bubble_sort{};

#include <algorithm>
#include <array>
#include <cassert>
#include <numeric>

int main()
{
    // Check that the bubble_sorter can sort any permutation
    // of an array of 8 values

    // Fill the collection in sorted order
    std::array<int, 8> collection;
    std::iota(collection.begin(), collection.end(), 0);

    // Projection to sort in descending order
    auto projection = [](int n) { return -n; };

    // For each possible permutation of collection
    do
    {
        auto to_sort = collection;
        // Bubble sort the collection
        auto last_it = bubble_sort(to_sort, projection);
        // Check that it is sorted in descending order
        assert(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<>{}));
        // Check that the correct iterator is returned
        assert(last_it == to_sort.end());
    } while (std::next_permutation(collection.begin(), collection.end()));
}
