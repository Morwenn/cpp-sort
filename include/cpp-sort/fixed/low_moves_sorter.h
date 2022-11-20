/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_FIXED_LOW_MOVES_SORTER_H_
#define CPPSORT_FIXED_LOW_MOVES_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/empty_sorter.h"
#include "../detail/minmax_element.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    template<std::size_t N>
    struct low_moves_sorter;

    namespace detail
    {
        template<std::size_t N>
        struct low_moves_sorter_impl
        {
            template<
                mstd::random_access_iterator Iterator,
                mstd::sentinel_for<Iterator> Sentinel,
                typename Compare = std::less<>,
                typename Projection = std::identity
            >
                requires is_projection_iterator_v<Projection, Iterator, Compare>
            auto operator()(Iterator first, Sentinel sentinel,
                            Compare compare={}, Projection projection={}) const
                -> Iterator
            {
                // There are specializations for N < 5, so unchecked_minmax_element
                // will always be passed at least 2 elements
                auto [min, max, last] = unchecked_minmax_element(first, sentinel, compare, projection);
                auto last_1 = std::prev(last);

                if (max == first && min == last_1) {
                    if (min == max) {
                        return last;
                    };
                    mstd::iter_swap(min, max);
                } else if (max == first) {
                    if (last_1 != max) {
                        mstd::iter_swap(last_1, max);
                    }
                    if (first != min) {
                        mstd::iter_swap(first, min);
                    }
                } else {
                    if (first != min) {
                        mstd::iter_swap(first, min);
                    }
                    if (last_1 != max) {
                        mstd::iter_swap(last_1, max);
                    }
                }

                ++first;
                low_moves_sorter<N-2u>{}(std::move(first), std::move(last_1),
                                         std::move(compare), std::move(projection));
                return last;
            }
        };

        template<>
        struct low_moves_sorter_impl<0u>:
            cppsort::detail::empty_sorter_impl
        {};

        template<>
        struct low_moves_sorter_impl<1u>:
            cppsort::detail::empty_sorter_impl
        {};
    }

    template<std::size_t N>
    struct low_moves_sorter:
        sorter_facade<detail::low_moves_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<low_moves_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        using is_always_stable = std::false_type;
    };

    template<>
    struct fixed_sorter_traits<low_moves_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

// Specializations of low_moves_sorter for some values of N
#include "../detail/low_moves/sort2.h"
#include "../detail/low_moves/sort3.h"
#include "../detail/low_moves/sort4.h"

#endif // CPPSORT_FIXED_LOW_MOVES_SORTER_H_
