/*
 * Copyright (c) 2015-2022 Morwenn
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
#include <cpp-sort/utility/as_function.h>

namespace detail
{
    template<typename ForwardIterator, typename Compare>
    auto bubble_sort(ForwardIterator first, std::size_t size, Compare compare)
        -> void
    {
        if (size < 2) return;

        auto&& comp = cppsort::utility::as_function(compare);

        while (--size) {
            auto current = first;
            auto next = std::next(current);
            for (std::size_t i = 0; i < size; ++i) {
                if (comp(*next, *current)) {
                    cppsort::mstd::iter_swap(current, next);
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
            cppsort::mstd::forward_iterator Iterator,
            typename Compare = std::less<>,
            typename = std::enable_if_t<
                not cppsort::is_projection_iterator_v<Compare, Iterator>
            >
        >
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> void
        {
            bubble_sort(first,
                        std::distance(first, last),
                        std::move(compare));
        }

        // Range overload
        template<
            cppsort::mstd::forward_range Range,
            typename Compare = std::less<>,
            typename = std::enable_if_t<
                not cppsort::is_projection_v<Compare, Range>
            >
        >
        auto operator()(Range&& range, Compare compare={}) const
            -> void
        {
            bubble_sort(cppsort::mstd::begin(range),
                        cppsort::mstd::distance(range),
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
        bubble_sort(to_sort, projection);
        // Check that it is sorted in descending order
        assert(std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<>{}));
    } while (std::next_permutation(collection.begin(), collection.end()));
}
