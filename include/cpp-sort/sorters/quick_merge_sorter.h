/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_QUICK_MERGE_SORTER_H_
#define CPPSORT_SORTERS_QUICK_MERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/size.h>
#include "../detail/iterator_traits.h"
#include "../detail/quick_merge_sort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct quick_merge_sorter_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            constexpr auto operator()(ForwardIterable&& iterable,
                                      Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<decltype(std::begin(iterable))>
                    >,
                    "quick_merge_sorter requires at least forward iterators"
                );

                quick_merge_sort(std::begin(iterable), std::end(iterable),
                                 utility::size(iterable),
                                 std::move(compare), std::move(projection));
            }

            template<
                mstd::forward_iterator Iterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            constexpr auto operator()(Iterator first, Iterator last,
                                      Compare compare={}, Projection projection={}) const
                -> void
            {
                using std::distance; // Hack for sized_iterator
                quick_merge_sort(std::move(first), std::move(last),
                                 distance(first, last),
                                 std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct quick_merge_sorter:
        sorter_facade<detail::quick_merge_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr quick_merge_sorter quick_merge_sort{};
}

#endif // CPPSORT_SORTERS_QUICK_MERGE_SORTER_H_
