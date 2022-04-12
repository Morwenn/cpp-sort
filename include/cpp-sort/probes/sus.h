/*
 * Copyright (c) 2021 Morwenn
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
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/longest_non_descending_subsequence.h"
#include "../detail/type_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct sus_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
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

    namespace
    {
        constexpr auto&& sus = utility::static_const<
            sorter_facade<detail::sus_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_SUS_H_
