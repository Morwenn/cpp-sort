/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_PROBES_REM_H_
#define CPPSORT_PROBES_REM_H_

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
        struct rem_impl
        {
            template<
                typename ForwardIterable,
                typename Compare = std::less<>,
                typename Projection = utility::identity,
                typename = cppsort::detail::enable_if_t<
                    is_projection_v<Projection, ForwardIterable, Compare> && (
                        cppsort::detail::is_detected_v<
                            cppsort::utility::detail::has_size_method_t,
                            ForwardIterable
                        > ||
                        cppsort::detail::is_bounded_array_v<
                            cppsort::detail::remove_cvref_t<ForwardIterable>
                        >
                    )
                >
            >
            auto operator()(ForwardIterable&& iterable,
                            Compare compare={}, Projection projection={}) const
                -> decltype(auto)
            {
                // While most algorithms use utility::size() for everything, we only
                // want to handle data structures that provide their own size() method
                // with the assumption that it's better than O(n) - which is at least
                // consistent as far as the standard library is concerned. We also
                // handle C arrays whose size is known and part of the type.
                auto res = cppsort::detail::longest_non_descending_subsequence<false>(
                    std::begin(iterable), std::end(iterable),
                    utility::size(iterable),
                    std::move(compare), std::move(projection)
                );
                auto lnds_size = res.second - res.first;
                return lnds_size >= 0 ? lnds_size : 0;
            }

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
                // We give 0 as a "dummy" value since it will be recomputed, but it
                // is also used by the non-random-access iterators version as the
                // initial value used for the size count
                auto res = cppsort::detail::longest_non_descending_subsequence<true>(
                    first, last, 0, std::move(compare), std::move(projection)
                );
                auto lnds_size = res.second - res.first;
                return lnds_size >= 0 ? lnds_size : 0;
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
        constexpr auto&& rem = utility::static_const<
            sorter_facade<detail::rem_impl>
        >::value;
    }
}}

#endif // CPPSORT_PROBES_REM_H_
