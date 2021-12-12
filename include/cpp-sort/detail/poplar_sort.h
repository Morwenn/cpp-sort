/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_POPLAR_SORT_H_
#define CPPSORT_DETAIL_POPLAR_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "bitops.h"
#include "insertion_sort.h"
#include "iterator_traits.h"

namespace cppsort
{
namespace detail
{
    template<typename RandomAccessIterator>
    struct poplar
    {
        RandomAccessIterator begin, end;
        std::make_unsigned_t<difference_type_t<RandomAccessIterator>> size;

        auto root() const
            -> RandomAccessIterator
        {
            auto res = end;
            return --res;
        }
    };

    template<typename RandomAccessIterator, typename Size,
             typename Compare, typename Projection>
    auto sift(RandomAccessIterator first, Size size,
              Compare compare, Projection projection)
        -> void
    {
        if (size < 2) return;

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        auto root = first + (size - 1);
        auto child_root1 = root - 1;
        auto child_root2 = first + (size / 2 - 1);

        while (true) {
            auto max_root = root;
            if (comp(proj(*max_root), proj(*child_root1))) {
                max_root = child_root1;
            }
            if (comp(proj(*max_root), proj(*child_root2))) {
                max_root = child_root2;
            }
            if (max_root == root) return;

            using utility::iter_swap;
            iter_swap(root, max_root);

            size /= 2;
            if (size < 2) return;

            root = max_root;
            child_root1 = root - 1;
            child_root2 = max_root - (size - size / 2);
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto relocate(const std::vector<poplar<RandomAccessIterator>>& poplars,
                  Compare compare, Projection projection)
        -> void
    {
        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        // Find the poplar with the bigger root
        // We can assume that there is always at least one poplar
        auto last = std::prev(poplars.end());
        auto bigger = last;
        for (auto it = poplars.begin() ; it != last ; ++it) {
            if (comp(proj(*bigger->root()), proj(*it->root()))) {
                bigger = it;
            }
        }

        if (bigger != last) {
            using utility::iter_swap;
            iter_swap(bigger->root(), last->root());
            sift(bigger->begin, bigger->size,
                 std::move(compare), std::move(projection));
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto make_poplar(RandomAccessIterator first, RandomAccessIterator last,
                     Compare compare, Projection projection)
        -> void
    {
        auto size = as_unsigned(last - first);
        if (size < 16) {
            // A sorted collection is a valid poplar heap;
            // when the heap is small, using insertion sort
            // should be faster
            insertion_sort(std::move(first), std::move(last),
                           std::move(compare), std::move(projection));
            return;
        }

        auto middle = first + size / 2;
        make_poplar(first, middle, compare, projection);
        make_poplar(middle, std::prev(last), compare, projection);
        sift(std::move(first), size, std::move(compare), std::move(projection));
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto poplar_sort(RandomAccessIterator first, RandomAccessIterator last,
                     Compare compare, Projection projection)
        -> void
    {
        using poplar_size_t = std::make_unsigned_t<difference_type_t<RandomAccessIterator>>;

        // Size of the unsorted subsequence
        poplar_size_t size = last - first;
        if (size < 2) return;

        std::vector<poplar<RandomAccessIterator>> poplars;
        // Harvey & Zatloukal, The Post-Order Heap:
        // [...] the number of trees, k, is at most floor(lg(n + 1)) + 1
        poplars.reserve(log2(size + 1) + 1);

        //
        // Size of the biggest poplar in the array, which always is a number
        // of the form 2^n - 1
        //
        // It's worth noting that the +1 never causes problems: we're only
        // using unsigned integers, so when size is the biggest representable
        // value for its type, size + 1 == 0 thanks to the behaviour of
        // unsigned overflow; hyperfloor(0) == 0, and subtracting 1 to that
        // gives back the biggest representable value, which happens to be
        // a number of the form 2^n - 1
        //
        poplar_size_t poplar_size = hyperfloor(size + 1u) - 1u;

        // Make the poplar heap
        auto it = first;
        do {
            if (poplar_size_t(last - it) >= poplar_size) {
                auto begin = it;
                auto end = it + poplar_size;
                make_poplar(begin, end, compare, projection);
                poplars.push_back({begin, end, poplar_size});
                it = end;
            } else {
                poplar_size = (poplar_size + 1) / 2 - 1;
            }
        } while (poplar_size > 0);

        // Sort the poplar heap
        do {
            // Find the greatest element, put it in place
            relocate(poplars, compare, projection);
            // If the last poplar had one element, destroy it
            if (poplars.back().size == 1) {
                poplars.pop_back();
                if (poplars.size() == 0) return;
                if (poplars.size() == 1) {
                    if (poplars.back().size == 1) return;
                    auto& back = poplars.back();
                    auto old_end = back.end;
                    poplar_size_t new_size = (back.size - 1) / 2;
                    auto middle = back.begin + new_size;
                    back.end = middle;
                    back.size = new_size;
                    poplars.push_back({middle, --old_end, new_size});
                }
            } else {
                auto& back = poplars.back();
                auto old_end = back.end;
                poplar_size_t new_size = (back.size - 1) / 2;
                auto middle = back.begin + new_size;
                back.end = middle;
                back.size = new_size;
                poplars.push_back({middle, --old_end, new_size});
            }

        } while (poplars.size() > 1);
    }
}}

#endif // CPPSORT_DETAIL_POPLAR_SORT_H_
