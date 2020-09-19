/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_DIS_H_
#define CPPSORT_PROBES_DIS_H_

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
#include <cpp-sort/utility/size.h>
#include <cpp-sort/utility/static_const.h>
#include "../detail/iterator_traits.h"

namespace cppsort
{
namespace probe
{
    namespace detail
    {
        template<typename ForwardIterator, typename Compare, typename Projection>
        auto dis_probe_algo(ForwardIterator first, ForwardIterator last,
                            cppsort::detail::difference_type_t<ForwardIterator> size,
                            Compare compare, Projection projection)
            -> ::cppsort::detail::difference_type_t<ForwardIterator>
        {
            using difference_type = cppsort::detail::difference_type_t<ForwardIterator>;
            auto&& comp = utility::as_function(compare);
            auto&& proj = utility::as_function(projection);

            if (size < 2) {
                return 0;
            }

            difference_type max_dist = 0;
            auto it1 = first;
            do {
                auto&& value = proj(*it1);

                difference_type dist = 1; // Distance between it1 and it2
                for (auto it2 = std::next(it1) ; it2 != last ; ++it2) {
                    if (comp(proj(*it2), value)) {
                        max_dist = std::max(max_dist, dist);
                    }
                    ++dist;
                }

                ++it1;
                --size;
            } while (max_dist < size);
            return max_dist;
        }

        struct dis_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = std::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare>
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                return dis_probe_algo(std::begin(iterable), std::end(iterable),
                                      utility::size(iterable),
                                      std::move(compare), std::move(projection));
            }

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
                -> decltype(auto)
            {
                return dis_probe_algo(first, last, std::distance(first, last),
                                      std::move(compare), std::move(projection));
            }
        };
    }

    namespace
    {
        constexpr auto&& dis = utility::static_const<
            sorter_facade<detail::dis_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_DIS_H_
