/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_ADAPTIVE_SHIVERS_SORTER_H_
#define CPPSORT_SORTERS_ADAPTIVE_SHIVERS_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/iterator_traits.h"
#include "../detail/timsort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct adaptive_shivers_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                >
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
                    "adaptive_shivers_sorter requires at least random-access iterators"
                );

                adaptive_shivers_sort(std::move(first), std::move(last),
                                      std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::true_type;
        };
    }

    struct adaptive_shivers_sorter:
        sorter_facade<detail::adaptive_shivers_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr adaptive_shivers_sorter adaptive_shivers_sort{};
}

#endif // CPPSORT_SORTERS_ADAPTIVE_SHIVERS_SORTER_H_
