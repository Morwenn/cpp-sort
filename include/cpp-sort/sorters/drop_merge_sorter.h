/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_DROP_MERGE_SORTER_H_
#define CPPSORT_SORTERS_DROP_MERGE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/adapters/drop_merge_adapter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/utility/static_const.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    struct drop_merge_sorter:
        drop_merge_adapter<pdq_sorter>
    {
        drop_merge_sorter() = default;
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    namespace
    {
        constexpr auto&& drop_merge_sort
            = utility::static_const<drop_merge_sorter>::value;
    }
}

#endif // CPPSORT_SORTERS_DROP_MERGE_SORTER_H_
