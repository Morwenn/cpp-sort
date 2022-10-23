/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_SUS_H_
#define CPPSORT_PROBES_SUS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/comparators/not_fn.h>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/type_traits.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/longest_non_descending_subsequence.h"

namespace cppsort::probe
{
    namespace detail
    {
        struct sus_impl
        {
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
                -> decltype(auto)
            {
                // We don't need the size information, so we can avoid
                // computing it altogether
                auto res = cppsort::detail::longest_non_descending_subsequence<false>(
                    first, last,
                    0, // Dummy value, not useful here
                    cppsort::not_fn(compare), std::move(projection)
                );
                return res.first > 0 ? res.first - 1 : 0;
            }

            template<typename Integer>
            static constexpr auto max_for_size(Integer n)
                -> Integer
            {
                return n == 0 ? 0 : n - 1;
            }
        };
    }

    inline constexpr sorter_facade<detail::sus_impl> sus{};
}

#endif // CPPSORT_PROBES_SUS_H_
