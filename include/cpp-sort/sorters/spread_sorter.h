/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPREAD_SORTER_H_
#define CPPSORT_SORTERS_SPREAD_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorters/spread_sorter/float_spread_sorter.h>
#include <cpp-sort/sorters/spread_sorter/integer_spread_sorter.h>
#include <cpp-sort/sorters/spread_sorter/string_spread_sorter.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    struct spread_sorter:
        hybrid_adapter<
            integer_spread_sorter,
            float_spread_sorter,
            string_spread_sorter
        >
    {};

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr spread_sorter spread_sort{};
}

#endif // CPPSORT_SORTERS_SPREAD_SORTER_H_
