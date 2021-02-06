/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_OSC_H_
#define CPPSORT_PROBES_OSC_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        struct osc_impl
        {
            template<
                typename ForwardIterator,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_iterator_v<Projection, ForwardIterator, Compare>
                >
            >
            auto operator()(ForwardIterator first, ForwardIterator last,
                            Compare compare={}, Projection projection={}) const
                -> cppsort::detail::difference_type_t<ForwardIterator>
            {
                using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
                auto&& comp = utility::as_function(compare);
                auto&& proj = utility::as_function(projection);

                if (first == last || std::next(first) == last)
                {
                    return 0;
                }

                difference_type count = 0;
                for (auto it = first ; it != last ; ++it)
                {
                    auto&& value = proj(*it);

                    auto current = first;
                    auto next = std::next(first);

                    while (next != last)
                    {
                        if (comp((std::min)(proj(*current), proj(*next)), value) &&
                            comp(value, (std::max)(proj(*current), proj(*next))))
                        {
                            ++count;
                        }

                        ++current;
                        ++next;
                    }
                }
                return count;
            }
        };
    }

    namespace
    {
        constexpr auto&& osc = utility::static_const<
            sorter_facade<detail::osc_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_OSC_H_
