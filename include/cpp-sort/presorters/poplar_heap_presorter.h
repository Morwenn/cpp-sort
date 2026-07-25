/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PRESORTERS_POPLAR_HEAP_PRESORTER_H_
#define CPPSORT_PRESORTERS_POPLAR_HEAP_PRESORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/poplar_sort.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct poplar_heap_presorter_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
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
                    "poplar_heap_presorter requires at least random-access iterators"
                );

                detail::make_poplar_heap(std::move(first), std::move(last),
                                         std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct poplar_heap_presorter:
        sorter_facade<detail::poplar_heap_presorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr poplar_heap_presorter poplar_heap_presort{};
}

#endif // CPPSORT_PRESORTERS_POPLAR_HEAP_PRESORTER_H_
