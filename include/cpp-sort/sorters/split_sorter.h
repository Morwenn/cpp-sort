/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_SORTERS_SPLIT_SORTER_H_
#define CPPSORT_SORTERS_SPLIT_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>
#include <type_traits>
#include <cpp-sort/adapters/split_adapter.h>
#include <cpp-sort/sorters/pdq_sorter.h>

namespace cppsort
{
    ////////////////////////////////////////////////////////////
    // Sorter

    struct split_sorter:
        split_adapter<pdq_sorter>
    {
        ////////////////////////////////////////////////////////////
        // Construction

        split_sorter() = default;

        ////////////////////////////////////////////////////////////
        // Sorter traits

        // Force it for legibility
        using iterator_category = std::random_access_iterator_tag;
    };

    ////////////////////////////////////////////////////////////
    // Sort function

    inline constexpr split_sorter split_sort{};
}

#endif // CPPSORT_SORTERS_SPLIT_SORTER_H_