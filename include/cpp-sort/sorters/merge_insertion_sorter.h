/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_MERGE_INSERTION_SORTER_H_
#define CPPSORT_SORTERS_MERGE_INSERTION_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/iterator_traits.h"
#include "../detail/merge_insertion_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct merge_insertion_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<is_projection_iterator_v<
                    Projection, RandomAccessIterator, Compare
                >>
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "merge_insertion_sorter requires at least random-access iterators"
                );

                merge_insertion_sort(std::move(first), std::move(last),
                                     std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct merge_insertion_sorter:
        sorter_facade<detail::merge_insertion_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr merge_insertion_sorter merge_insertion_sort{};
}

#endif // CPPSORT_SORTERS_MERGE_INSERTION_SORTER_H_
