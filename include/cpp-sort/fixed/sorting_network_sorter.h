/*
 * Copyright (c) 2015-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_
#define CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cstddef>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include "../detail/empty_sorter.h"

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Adapter

    namespace detail
    {
        template<std::size_t N>
        struct sorting_network_sorter_impl
        {
            static_assert(
                N && false,
                "sorting_network_sorter has no specialization for this size of N"
            );
        };

        template<>
        struct sorting_network_sorter_impl<0>:
            cppsort::detail::empty_network_sorter_impl
        {};

        template<>
        struct sorting_network_sorter_impl<1>:
            cppsort::detail::empty_network_sorter_impl
        {};
    }

    template<std::size_t N>
    struct sorting_network_sorter:
        sorter_facade<detail::sorting_network_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // sorter_traits

    template<std::size_t N>
    struct sorter_traits<sorting_network_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    template<>
    struct sorter_traits<sorting_network_sorter<0>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::true_type;
    };

    template<>
    struct sorter_traits<sorting_network_sorter<1>>
    {
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::true_type;
    };

    ////////////////////////////////////////////////////////////
    // fixed_sorter_traits

    template<>
    struct fixed_sorter_traits<sorting_network_sorter>
    {
        using domain = std::make_index_sequence<33>;
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

// Common includes for specializations
#include <array>
#include <functional>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/sorting_networks.h>
#include "../detail/attributes.h"
#include "../detail/swap_if.h"
#include "../detail/type_traits.h"

// Explicit specializations of sorting_network_sorter
#include "../detail/sorting_network/sort2.h"
#include "../detail/sorting_network/sort3.h"
#include "../detail/sorting_network/sort4.h"
#include "../detail/sorting_network/sort5.h"
#include "../detail/sorting_network/sort6.h"
#include "../detail/sorting_network/sort7.h"
#include "../detail/sorting_network/sort8.h"
#include "../detail/sorting_network/sort9.h"
#include "../detail/sorting_network/sort10.h"
#include "../detail/sorting_network/sort11.h"
#include "../detail/sorting_network/sort12.h"
#include "../detail/sorting_network/sort13.h"
#include "../detail/sorting_network/sort14.h"
#include "../detail/sorting_network/sort15.h"
#include "../detail/sorting_network/sort16.h"
#include "../detail/sorting_network/sort17.h"
#include "../detail/sorting_network/sort18.h"
#include "../detail/sorting_network/sort19.h"
#include "../detail/sorting_network/sort20.h"
#include "../detail/sorting_network/sort21.h"
#include "../detail/sorting_network/sort22.h"
#include "../detail/sorting_network/sort23.h"
#include "../detail/sorting_network/sort24.h"
#include "../detail/sorting_network/sort25.h"
#include "../detail/sorting_network/sort26.h"
#include "../detail/sorting_network/sort27.h"
#include "../detail/sorting_network/sort28.h"
#include "../detail/sorting_network/sort29.h"
#include "../detail/sorting_network/sort30.h"
#include "../detail/sorting_network/sort31.h"
#include "../detail/sorting_network/sort32.h"
#include "../detail/sorting_network/sort33.h"
#include "../detail/sorting_network/sort34.h"
#include "../detail/sorting_network/sort35.h"
#include "../detail/sorting_network/sort36.h"
#include "../detail/sorting_network/sort37.h"
#include "../detail/sorting_network/sort38.h"
#include "../detail/sorting_network/sort39.h"
#include "../detail/sorting_network/sort40.h"
#include "../detail/sorting_network/sort41.h"
#include "../detail/sorting_network/sort42.h"
#include "../detail/sorting_network/sort43.h"
#include "../detail/sorting_network/sort44.h"
#include "../detail/sorting_network/sort45.h"
#include "../detail/sorting_network/sort46.h"
#include "../detail/sorting_network/sort47.h"
#include "../detail/sorting_network/sort48.h"
#include "../detail/sorting_network/sort49.h"
#include "../detail/sorting_network/sort50.h"
#include "../detail/sorting_network/sort51.h"
#include "../detail/sorting_network/sort52.h"
#include "../detail/sorting_network/sort53.h"
#include "../detail/sorting_network/sort54.h"
#include "../detail/sorting_network/sort55.h"
#include "../detail/sorting_network/sort56.h"
#include "../detail/sorting_network/sort57.h"
#include "../detail/sorting_network/sort58.h"
#include "../detail/sorting_network/sort59.h"
#include "../detail/sorting_network/sort60.h"
#include "../detail/sorting_network/sort61.h"
#include "../detail/sorting_network/sort62.h"
#include "../detail/sorting_network/sort63.h"
#include "../detail/sorting_network/sort64.h"

#endif // CPPSORT_FIXED_SORTING_NETWORK_SORTER_H_
