/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_OLD_DEFAULT_SORTER_H_
#define CPPSORT_TESTSUITE_OLD_DEFAULT_SORTER_H_

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

////////////////////////////////////////////////////////////
// Old default_sorter
//
// A copy of the library's old default_sorter. While the
// sorter in itself was a rather bad idea for a user-facing
// component, it was still a good class to test the nesting
// of adapters on the same sorter. It helped to harden the
// test suite and to find bugs in the adapters used, so
// here it is again to preserve the old tests.

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


#endif // CPPSORT_TESTSUITE_OLD_DEFAULT_SORTER_H_
