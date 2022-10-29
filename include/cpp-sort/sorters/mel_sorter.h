/*
 * Copyright (c) 2018-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_MEL_SORTER_H_
#define CPPSORT_SORTERS_MEL_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/iterator_traits.h"
#include "../detail/melsort.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    namespace detail
    {
        struct mel_sorter_impl
        {
            template<
                mstd::forward_range Range,
                typename Compare = std::less<>,
                typename Projection = std::identity,
                typename = mstd::enable_if_t<
                    is_projection_v<Projection, Range, Compare>
                >
            >
            auto operator()(Range&& range, Compare compare={}, Projection projection={}) const
                -> void
            {
                melsort(mstd::begin(range), mstd::end(range),
                        mstd::distance(range),
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
            auto operator()(Iterator first, Iterator last,
                            Compare compare={}, Projection projection={}) const
                -> void
            {
                auto dist = std::distance(first, last);
                melsort(std::move(first), std::move(last), dist,
                        std::move(compare), std::move(projection));
            }

            ////////////////////////////////////////////////////////////
            // Sorter traits

            using iterator_category = std::forward_iterator_tag;
            using is_always_stable = std::false_type;
        };
    }

    struct mel_sorter:
        sorter_facade<detail::mel_sorter_impl>
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr mel_sorter mel_sort{};
}

#ifdef CPPSORT_ADAPTERS_CONTAINER_AWARE_ADAPTER_DONE_
#include "../detail/container_aware/mel_sort.h"
#endif

#endif // CPPSORT_SORTERS_MEL_SORTER_H_
