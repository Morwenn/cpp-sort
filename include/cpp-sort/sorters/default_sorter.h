/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_DEFAULT_SORTER_H_
#define CPPSORT_SORTERS_DEFAULT_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/fixed/low_comparisons_sorter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/quick_sorter.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Stable sorter

    // Declare the specialization first to avoid deprecation
    // warning at definition time
    struct default_sorter;

    template<>
    struct stable_adapter<default_sorter>:
        merge_sorter
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(const default_sorter&) noexcept:
            stable_adapter()
        {}
    };

    ////////////////////////////////////////////////////////////
    // Unstable sorter

    struct CPPSORT_DEPRECATED("default_sorter is deprecated and will be removed in version 2.0.0")
    default_sorter:
        self_sort_adapter<
            hybrid_adapter<
                small_array_adapter<
                    low_comparisons_sorter,
                    std::make_index_sequence<14u>
                >,
                quick_sorter,
                pdq_sorter
            >
        >
    {};
}

#endif // CPPSORT_SORTERS_DEFAULT_SORTER_H_
