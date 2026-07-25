/*
 * Copyright (c) 2015-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_ALGORITHM_H_
#define CPPSORT_TESTSUITE_ALGORITHM_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/detail/iterator_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace helpers
{
    template<
        typename Iterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    constexpr auto is_sorted(Iterator first, Iterator last,
                             Compare compare={}, Projection projection={})
        -> bool
    {
        auto&& comp = cppsort::utility::as_function(compare);
        auto&& proj = cppsort::utility::as_function(projection);

        if (first == last) {
            return true;
        }

        auto next = first;
        while (++next != last) {
            if (comp(proj(*next), proj(*first))) {
                return false;
            }
            ++first;
        }
        return true;
    }

    template<
        typename ForwardIterator,
        typename T,
        typename Projection = cppsort::utility::identity
    >
    auto iota(ForwardIterator first, ForwardIterator last,
              T value, Projection projection={})
        -> void
    {
        auto&& proj = cppsort::utility::as_function(projection);

        while (first != last)
        {
            proj(*first++) = value;
            ++value;
        }
    }

    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    auto is_poplar_heap_until(RandomAccessIterator first, RandomAccessIterator last,
                              Compare compare={}, Projection projection={})
        -> RandomAccessIterator
    {
        using poplar_size_t = std::make_unsigned_t<
            cppsort::detail::difference_type_t<RandomAccessIterator>
        >;

        auto&& comp = cppsort::utility::as_function(compare);
        auto&& proj = cppsort::utility::as_function(projection);

        if (last - first < 2) {
            return last;
        }

        // Determines the "level" of the poplars seen so far; the log2 of this
        // variable will be used to make the binary carry sequence
        poplar_size_t poplar_level = 1;

        auto it = first;
        auto next = std::next(it);
        while (true) {
            poplar_size_t poplar_size = 1;

            // Bit trick iterate without actually having to compute log2(poplar_level)
            for (auto i = (poplar_level & -poplar_level) >> 1 ; i != 0 ; i >>= 1) {
                // Beginning and size of the poplar to track
                it -= poplar_size;
                poplar_size = 2 * poplar_size + 1;

                // Check poplar property against child roots
                auto root = it + (poplar_size - 1);
                auto child_root1 = std::prev(root);
                if (comp(proj(*root), proj(*child_root1))) {
                    return next;
                }
                auto child_root2 = it + (poplar_size / 2 - 1);
                if (compare(proj(*root), proj(*child_root2))) {
                    return next;
                }

                if (next == last) return last;
                ++next;
            }

            if (next == last) return last;
            it = next;
            ++next;
            ++poplar_level;
        }
    }

    template<
        typename RandomAccessIterator,
        typename Compare = std::less<>,
        typename Projection = cppsort::utility::identity
    >
    auto is_poplar_heap(RandomAccessIterator first, RandomAccessIterator last,
                        Compare compare={}, Projection projection={})
        -> bool
    {
        return is_poplar_heap_until(
            first, last, std::move(compare), std::move(projection)
        ) == last;
    }
}

#endif // CPPSORT_TESTSUITE_ALGORITHM_H_
