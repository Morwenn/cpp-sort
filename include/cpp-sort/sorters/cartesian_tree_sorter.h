/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_CARTESIAN_TREE_SORTER_H_
#define CPPSORT_SORTERS_CARTESIAN_TREE_SORTER_H_

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
#include <cpp-sort/utility/size.h>
#include "../detail/cartesian_tree_sort.h"
#include "../detail/iterator_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct cartesian_tree_sorter_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<decltype(std::begin(iterable))>
                    >,
                    "cartesian_tree_sorter requires at least forward iterators"
                );

                cartesian_tree_sort(std::begin(iterable), std::end(iterable),
                                    utility::size(iterable),
                                    std::move(compare), std::move(projection));
            }

            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >,
                    "cartesian_tree_sorter requires at least forward iterators"
                );

                auto size = std::distance(first, last);
                cartesian_tree_sort(std::move(first), std::move(last), size,
                                    std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct cartesian_tree_sorter:
        sorter_facade<detail::cartesian_tree_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr cartesian_tree_sorter cartesian_tree_sort{};
}

#endif // CPPSORT_SORTERS_CARTESIAN_TREE_SORTER_H_
