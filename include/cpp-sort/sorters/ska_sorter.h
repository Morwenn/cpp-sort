/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SKA_SORTER_H_
#define CPPSORT_SORTERS_SKA_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include "../detail/iterator_traits.h"
#include "../detail/ska_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct ska_sorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Projection projection={}) const
                -> mstd::enable_if_t<detail::is_ska_sortable_v<
                    projected_t<RandomAccessIterator, Projection>
                >>
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >,
                    "ska_sorter requires at least random-access iterators"
                );

                ska_sort(std::move(first), std::move(last), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct ska_sorter:
        sorter_facade<detail::ska_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr ska_sorter ska_sort{};
}

#endif // CPPSORT_SORTERS_SKA_SORTER_H_
