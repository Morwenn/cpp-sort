/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_D_ARY_HEAP_SORTER_H_
#define CPPSORT_SORTERS_D_ARY_HEAP_SORTER_H_

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
#include "../detail/d_ary_heapsort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        template<int D>
        struct d_ary_heap_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_iterator_v<Projection, Iterator, Compare>
                >
            >
            constexpr auto operator()(Iterator first, Sentinel last,
                                      Compare compare={}, Projection projection={}) const
                -> void
            {
                auto last_it = mstd::next(first, std::move(last));
                d_ary_heapsort<D>(std::move(first), std::move(last_it),
                                 std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::random_access_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    template<int D>
    struct d_ary_heap_sorter:
        sorter_facade<detail::d_ary_heap_sorter_impl<D>>
    {
        static_assert(D >= 2, "d_ary_heap_sorter must be instantiated with D >= 2");
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    template<int D>
    inline constexpr d_ary_heap_sorter<D> d_ary_heap_sort{};
}

#endif // CPPSORT_SORTERS_D_ARY_HEAP_SORTER_H_
