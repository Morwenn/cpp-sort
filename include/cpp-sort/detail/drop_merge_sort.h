/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
    The MIT license (MIT)

    A C++ reimplementation of a drop-merge sort, originally made by Emil Ernerfeldt:
    https://github.com/emilk/drop-merge-sort

    There are two versions of this function.

    The first one is identical to the reference Rust implementation.
    It doesn't support uncopyable types.
    Here, it's used only for trivially copyable types.
    (just like the Rust version, which required the Copy trait).

    The second version is copy-free, so it supports noncopyable types like std::unique_ptr.
    It's also more efficient for types with expensive copying, like std::string.
    However, being copy-free involves some extra bookkeeping, so for trivial types
    it's a bit less efficient than the original function - which is why I use both versions.
*/
#ifndef CPPSORT_DETAIL_DROP_MERGE_SORT_H_
#define CPPSORT_DETAIL_DROP_MERGE_SORT_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/utility/as_function.h>
#include "iterator_traits.h"

namespace cppsort::detail
{
    template<
        typename BidirectionalIterator,
        typename Sentinel,
        typename Compare,
        typename Projection,
        typename Sorter
    >
    auto drop_merge_sort(BidirectionalIterator first, Sentinel last,
                         Compare compare, Projection projection, Sorter&& sorter)
        -> BidirectionalIterator
    {
        using difference_type = mstd::iter_difference_t<BidirectionalIterator>;
        using rvalue_type = rvalue_type_t<BidirectionalIterator>;

        // Configuration variables
        constexpr bool double_comparison = true;
        constexpr difference_type recency = 8;

        if (first == last) {
            return first;
        }
        auto second = std::next(first);
        if (second == last) {
            return second;
        }

        auto&& comp = utility::as_function(compare);
        auto&& proj = utility::as_function(projection);

        std::vector<rvalue_type> dropped;
        difference_type num_dropped_in_row = 0;
        auto write = first;
        auto read = first;

        do {
            if (first != write && comp(proj(*read), proj(*std::prev(write)))) {

                if (double_comparison && num_dropped_in_row == 0 && write != second &&
                    not comp(proj(*read), proj(*std::prev(write, 2)))) {
                    dropped.push_back(mstd::iter_move(std::prev(write)));
                    *std::prev(write) = mstd::iter_move(read);
                    ++read;
                    continue;
                }

                if (num_dropped_in_row < recency) {
                    dropped.push_back(mstd::iter_move(read));
                    ++read;
                    ++num_dropped_in_row;
                } else {
                    for (difference_type i = 0; i < num_dropped_in_row; ++i) {
                        --read;
                        if constexpr (not std::is_trivially_copyable_v<rvalue_type>) {
                            // If the value is trivially copyable, then it shouldn't have
                            // been modified by the call to iter_move, and the original
                            // value is still fully where it should be
                            *read = std::move(*std::prev(dropped.end()));
                        }
                        dropped.pop_back();
                    }

                    --write;
                    dropped.push_back(mstd::iter_move(write));

                    num_dropped_in_row = 0;
                }
            } else {
                if constexpr (std::is_trivially_copyable_v<rvalue_type>) {
                    // If the type is trivially copyable, the potential self-move
                    // should not trigger any issue
                    *write = mstd::iter_move(read);
                } else {
                    if (read != write) {
                        *write = mstd::iter_move(read);
                    }
                }
                ++read;
                ++write;
                num_dropped_in_row = 0;
            }
        } while (read != last);

        // Don't bother with merging if there is nothing to merge
        if (dropped.empty()) {
            return read; // == last
        }

        // Sort the dropped elements
        std::forward<Sorter>(sorter)(dropped.begin(), dropped.end(), compare, projection);

        auto back = read;
        do {
            auto& last_dropped = dropped.back();

            while (first != write && comp(proj(last_dropped), proj(*std::prev(write)))) {
                --back;
                --write;
                *back = mstd::iter_move(write);
            }
            --back;
            *back = std::move(last_dropped);
            dropped.pop_back();
        } while (not dropped.empty());

        return read; // == last
    }
}

#endif // CPPSORT_DETAIL_DROP_MERGE_SORT_H_
