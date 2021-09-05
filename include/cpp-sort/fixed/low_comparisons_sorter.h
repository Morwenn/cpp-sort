/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_
#define CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_

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
        struct low_comparisons_sorter_impl
        {
            static_assert(
                N && false,
                "low_comparisons_sorter has no specialization for this size of N"
            );
        };

        template<>
        struct low_comparisons_sorter_impl<0u>:
            cppsort::detail::empty_sorter_impl
        {};

        template<>
        struct low_comparisons_sorter_impl<1u>:
            cppsort::detail::empty_sorter_impl
        {};
    }

    template<std::size_t N>
    struct low_comparisons_sorter:
        sorter_facade<detail::low_comparisons_sorter_impl<N>>
    {};

    ////////////////////////////////////////////////////////////
    // Sorter traits

    template<std::size_t N>
    struct sorter_traits<low_comparisons_sorter<N>>
    {
        using iterator_category = std::random_access_iterator_tag;

        // Some of the algorithms are stable, some other are not,
        // the stability *could* be documented depending on which
        // fixed-size algorithms are used, but it would be lots of
        // work...
        using is_always_stable = std::false_type;
    };

    template<>
    struct fixed_sorter_traits<low_comparisons_sorter>
    {
        using domain = std::make_index_sequence<14>;
        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };
}

// Common includes for specializations
#include <functional>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include "../detail/front_insert.h"
#include "../detail/swap_if.h"
#include "../detail/type_traits.h"

// Specializations of low_comparisons_sorter for some values of N
#include "../detail/low_comparisons/sort2.h"
#include "../detail/low_comparisons/sort3.h"
#include "../detail/low_comparisons/sort4.h"
#include "../detail/low_comparisons/sort5.h"
#include "../detail/low_comparisons/sort6.h"
#include "../detail/low_comparisons/sort7.h"
#include "../detail/low_comparisons/sort8.h"
#include "../detail/low_comparisons/sort9.h"
#include "../detail/low_comparisons/sort10.h"
#include "../detail/low_comparisons/sort11.h"
#include "../detail/low_comparisons/sort12.h"
#include "../detail/low_comparisons/sort13.h"

#endif // CPPSORT_FIXED_LOW_COMPARISONS_SORTER_H_
