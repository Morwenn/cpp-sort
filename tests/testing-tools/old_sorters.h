/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_OLD_SORTERS_H_
#define CPPSORT_TESTSUITE_OLD_SORTERS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <utility>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/fixed/low_comparisons_sorter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/quick_sorter.h>

// Copies of the library's old sorters. While these sorters have
// since been superseded and/or were rather bad ideas to start
// with, keeping them around allows to harden the test suite:
// they can indeed be implemented as compositions of adapters
// that are otherwise not as tested as most sorters.

////////////////////////////////////////////////////////////
// old_default_sorter

struct old_default_sorter:
    cppsort::self_sort_adapter<
        cppsort::hybrid_adapter<
            cppsort::small_array_adapter<
                cppsort::low_comparisons_sorter,
                std::make_index_sequence<14u>
            >,
            cppsort::quick_sorter,
            cppsort::pdq_sorter
        >
    >
{};

namespace cppsort
{
    template<>
    struct stable_adapter<::old_default_sorter>:
        cppsort::merge_sorter
    {
        stable_adapter() = default;

        constexpr explicit stable_adapter(const ::old_default_sorter&) noexcept:
            stable_adapter()
        {}
    };
}

#endif // CPPSORT_TESTSUITE_OLD_SORTERS_H_
