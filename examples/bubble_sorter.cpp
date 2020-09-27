/*
 * Copyright (c) 2015-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>

namespace detail
{
    template<
        typename ForwardIterator,
        typename StrictWeakOrdering
    >
    auto bubble_sort(ForwardIterator first, std::size_t size,
                     StrictWeakOrdering compare)
        -> void
    {
        if (size < 2) return;

        auto&& comp = cppsort::utility::as_function(compare);

        while (--size) {
            ForwardIterator current = first;
            ForwardIterator next = std::next(current);
            for (std::size_t i = 0 ; i < size ; ++i) {
                if (comp(*next, *current)) {
                    using cppsort::utility::iter_swap;
                    iter_swap(current, next);
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
            typename = std::enable_if_t<not cppsort::is_projection_iterator_v<
                StrictWeakOrdering, ForwardIterator
            >>
        >
        auto operator()(ForwardIterator first, ForwardIterator last,
                        StrictWeakOrdering compare={}) const
            -> void
        {
            static_assert(
                std::is_base_of<
                    std::forward_iterator_tag,
                    typename std::iterator_traits<ForwardIterator>::iterator_category
                >::value,
                "bubble_sorter requires at least forward iterators"
            );

            bubble_sort(first, std::distance(first, last),
                        compare);
        }

        // Iterable overload
        template<
            typename ForwardIterable,
            typename StrictWeakOrdering = std::less<>,
            typename = std::enable_if_t<not cppsort::is_projection_v<
                StrictWeakOrdering, ForwardIterable
            >>
        >
        auto operator()(ForwardIterable&& iterable, StrictWeakOrdering compare={}) const
            -> void
        {
            static_assert(
                std::is_base_of<
                    std::forward_iterator_tag,
                    typename std::iterator_traits<decltype(std::begin(iterable))>::iterator_category
                >::value,
                "bubble_sorter requires at least forward iterators"
            );

            bubble_sort(std::begin(iterable), cppsort::utility::size(iterable),
                        compare);
        }

        // Sorter traits
        using iterator_category = std::forward_iterator_tag;
        using is_always_stable = std::true_type;
    };
}

struct bubble_sorter:
    cppsort::sorter_facade<detail::bubble_sorter_impl>
{};

namespace
{
    constexpr auto&& bubble_sort
        = cppsort::utility::static_const<bubble_sorter>::value;
}

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
    std::iota(std::begin(collection), std::end(collection), 0);

    // Projection to sort in descending order
    auto projection = [](int n) { return -n; };

    // For each possible permutation of collection
    do
    {
        auto to_sort = collection;
        // Bubble sort the collection
        bubble_sort(to_sort, projection);
        // Check that it is sorted in descending order
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort), std::greater<>{}));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));
}
