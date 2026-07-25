/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PRESORTERS_MIN_HEAP_PRESORTER_H_
#define CPPSORT_PRESORTERS_MIN_HEAP_PRESORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include "../detail/heapsort.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct min_heap_presorter_impl
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
                    "min_heap_presorter requires at least random-access iterators"
                );

                make_heap(std::move(first), std::move(last),
                          cppsort::flip(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct min_heap_presorter:
        sorter_facade<detail::min_heap_presorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr min_heap_presorter min_heap_presort{};
}

#endif // CPPSORT_PRESORTERS_MIN_HEAP_PRESORTER_H_
