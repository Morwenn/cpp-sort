/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_DETAIL_EMPTY_SORTER_H_
#define CPPSORT_DETAIL_EMPTY_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <array>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/sorting_networks.h>
#include "attributes.h"
#include "type_traits.h"

namespace cppsort
{
namespace detail
{
    ////////////////////////////////////////////////////////////
    // Basic empty fixed-size "sorter", generally the one used
    // by fixed-size sorters of size 0 or 1, which don't need
    // to reorder anything

    struct empty_sorter_impl
    {
        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = utility::identity,
            typename = detail::enable_if_t<is_projection_iterator_v<
                Projection, ForwardIterator, Compare
            >>
        >
        constexpr auto operator()(ForwardIterator, ForwardIterator,
                                  Compare={}, Projection={}) const noexcept
            -> void
        {}
    };

    ////////////////////////////////////////////////////////////
    // Dedicated empty sorter for sorting networks, providing
    // additional sorting network-specific functions

    struct empty_network_sorter_impl:
        empty_sorter_impl
    {
        template<typename DifferenceType=std::ptrdiff_t>
        CPPSORT_ATTRIBUTE_NODISCARD
        static constexpr auto index_pairs()
            -> std::array<utility::index_pair<DifferenceType>, 0>
        {
            return {};
        }
    };
}}

#endif // CPPSORT_DETAIL_EMPTY_SORTER_H_
