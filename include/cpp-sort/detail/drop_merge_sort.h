/*
    The MIT license (MIT)

    A C++ reimplementation of a drop-merge sort, originally made by Emil Ernerfeldt:
    https://github.com/emilk/drop-merge-sort

    Modified in 2017 by Morwenn for inclusion into cpp-sort

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
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/iter_move.h>
#include "iterator_traits.h"
#include "pdqsort.h"

namespace cppsort
{
namespace detail
{
    constexpr static bool double_comparison = true;

    // with-trivial-copies version
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto drop_merge_sort(RandomAccessIterator begin, RandomAccessIterator end,
                         Compare compare, Projection projection,
                         std::true_type)
        -> void
    {
        std::size_t size = std::distance(begin, end);
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        using value_type = value_type_t<RandomAccessIterator>;
        std::vector<value_type> dropped;

        std::size_t num_dropped_in_row = 0;
        auto write = begin;
        auto read = begin;

        static constexpr std::size_t recency = 8;

        while (read != end) {
            if (begin != write && compare(proj(*read), proj(*std::prev(write)))) {

                if (double_comparison && num_dropped_in_row == 0 && write > begin+1 &&
                    not compare(proj(*read), proj(*(write-2)))) {
                    dropped.push_back(*(write-1));
                    *(write-1) = *read;
                    ++read;
                    continue;
                }

                if (num_dropped_in_row < recency) {
                    dropped.push_back(*read);
                    ++read;
                    ++num_dropped_in_row;
                } else {
                    for (std::size_t i = 0 ; i < num_dropped_in_row ; ++i) {
                        dropped.pop_back();
                    }
                    read -= num_dropped_in_row;

                    --write;
                    dropped.push_back(*write);

                    num_dropped_in_row = 0;
                }
            } else {
                *write = std::move(*read);
                ++read;
                ++write;
                num_dropped_in_row = 0;
            }
        }

        // Sort the dropped elements
        pdqsort(std::begin(dropped), std::end(dropped), compare, projection);

        auto back = end;

        while (not dropped.empty()) {
            auto& last_dropped = dropped.back();

            while (begin != write && compare(proj(last_dropped), proj(*std::prev(write)))) {
                --back;
                --write;
                *back = std::move(*write);
            }
            --back;
            *back = std::move(last_dropped);
            dropped.pop_back();
        }
    }

    // move-only version
    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto drop_merge_sort(RandomAccessIterator begin, RandomAccessIterator end,
                         Compare compare, Projection projection,
                         std::false_type)
        -> void
    {
        using utility::iter_move;

        std::size_t size = std::distance(begin, end);
        if (size < 2) return;

        auto&& proj = utility::as_function(projection);

        using rvalue_reference = std::decay_t<rvalue_reference_t<RandomAccessIterator>>;
        std::vector<rvalue_reference> dropped;

        std::size_t num_dropped_in_row = 0;
        auto write = begin;
        auto read = begin;

        static constexpr std::size_t recency = 8;

        while (read != end) {
            if (begin != write && compare(proj(*read), proj(*std::prev(write)))) {

                if (double_comparison && num_dropped_in_row == 0 && write > begin + 1 &&
                    not compare(proj(*read), proj(*(write-2)))) {
                    dropped.push_back(iter_move(std::prev(write)));
                    *(write-1) = iter_move(read);
                    ++read;
                    continue;
                }

                if (num_dropped_in_row < recency) {
                    dropped.push_back(iter_move(read));
                    ++read;
                    ++num_dropped_in_row;
                } else {
                    for (std::size_t i = 0 ; i < num_dropped_in_row ; ++i) {
                        --read;
                        *read = std::move(*std::prev(dropped.end()));
                        dropped.pop_back();
                    }

                    --write;
                    dropped.push_back(iter_move(write));

                    num_dropped_in_row = 0;
                }
            } else {
                *write = iter_move(read);
                ++read;
                ++write;
                num_dropped_in_row = 0;
            }
        }

        // Sort the dropped elements
        pdqsort(std::begin(dropped), std::end(dropped), compare, projection);

        auto back = end;

        while (not dropped.empty()) {
            auto& last_dropped = dropped.back();

            while (begin != write && compare(proj(last_dropped), proj(*std::prev(write)))) {
                --back;
                --write;
                *back = iter_move(write);
            }
            --back;
            *back = std::move(last_dropped);
            dropped.pop_back();
        }
    }

    template<typename RandomAccessIterator, typename Compare, typename Projection>
    auto drop_merge_sort(RandomAccessIterator first, RandomAccessIterator last,
                         Compare compare, Projection projection)
        -> void
    {
        using value_type = value_type_t<RandomAccessIterator>;
        drop_merge_sort(std::move(first), std::move(last),
                        std::move(compare), std::move(projection),
                        std::is_trivially_copyable<value_type>{});
    }
}}

#endif // CPPSORT_DETAIL_DROP_MERGE_SORT_H_
