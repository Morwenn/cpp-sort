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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/d_ary_heapsort.h"
#include "../detail/iterator_traits.h"
#include "../detail/type_traits.h"

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
                    std::is_base_of<
                        iterator_category,
                        iterator_category_t<RandomAccessIterator>
                    >::value,
                    "d_ary_heap_sorter requires at least random-access iterators"
                );

                d_ary_heapsort<D>(std::move(first), std::move(last),
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

    namespace
    {
        template<int D>
        constexpr auto&& d_ary_heap_sort
            = utility::static_const<d_ary_heap_sorter<D>>::value;
    }
}

#endif // CPPSORT_SORTERS_D_ARY_HEAP_SORTER_H_
