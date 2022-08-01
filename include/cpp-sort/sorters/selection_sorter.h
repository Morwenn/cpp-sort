/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SELECTION_SORTER_H_
#define CPPSORT_SORTERS_SELECTION_SORTER_H_

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
#include "../detail/selection_sort.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct selection_sorter_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            constexpr auto operator()(ForwardIterator first, ForwardIterator last,
                                      Compare compare={}, Projection projection={}) const
                -> void
            {
                static_assert(
                    std::is_base_of_v<
                        iterator_category,
                        iterator_category_t<ForwardIterator>
                    >,
                    "selection_sorter requires at least forward iterators"
                );

                selection_sort(std::move(first), std::move(last),
                               std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct selection_sorter:
        sorter_facade<detail::selection_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr selection_sorter selection_sort{};
}

#ifdef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_
#include "../detail/container_aware/selection_sort.h"
#endif

#define CPPSORT_SORTERS_SELECTION_SORTER_DONE_

#endif // CPPSORT_SORTERS_SELECTION_SORTER_H_
