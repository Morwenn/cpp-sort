/*
 * Copyright (c) 2017-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_DROP_MERGE_SORTER_H_
#define CPPSORT_SORTERS_DROP_MERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/adapters/drop_merge_adapter.h>
#include <cpp-sort/sorters/pdq_sorter.h>

namespace cppsort
{
    namespace deprecated
    {
        struct drop_merge_sorter:
            drop_merge_adapter<pdq_sorter>
        {
            drop_merge_sorter() = default;
        };
    }

    ////////////////////////////////////////////////////////////
    // Sorter

    struct CPPSORT_DEPRECATED("drop_merge_sorter is deprecated and will be removed in version 2.0.0, use drop_merge_adapter(pdq_sort) instead")
    drop_merge_sorter:
        drop_merge_adapter<pdq_sorter>
    {
        drop_merge_sorter() = default;
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    CPPSORT_DEPRECATED("drop_merge_sort is deprecated and will be removed in version 2.0.0, use drop_merge_adapter(pdq_sort) instead")
    inline constexpr drop_merge_sorter drop_merge_sort{};
}

#endif // CPPSORT_SORTERS_DROP_MERGE_SORTER_H_
