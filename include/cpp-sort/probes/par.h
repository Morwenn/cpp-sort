/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_PAR_H_
#define CPPSORT_PROBES_PAR_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/is_p_sorted.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct par_impl
        {
            template<
                typename RandomAccessIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
                >
            >
            auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<RandomAccessIterator>
            {
                auto size = last - first;

                auto res = 0;
                while (size > 0) {
                    auto p = res;
                    p += size / 2;
                    if (cppsort::detail::is_p_sorted(first, last, p, compare, projection)) {
                        size /= 2;
                    } else {
                        res = ++p;
                        size -= size / 2 + 1;
                    }
                }
                return res;
            }
        };
    }

    namespace
    {
        constexpr auto&& par = utility::static_const<
            sorter_facade<detail::par_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_PAR_H_
