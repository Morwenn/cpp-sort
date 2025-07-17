/*
 * Copyright (c) 2019-2025 Morwenn
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
#include "../detail/config.h"

namespace cppsort
{
    namespace deprecated
    {
        struct split_sorter:
            split_adapter<pdq_sorter>
        {
            split_sorter() = default;
            using iterator_category = std::random_access_iterator_tag;
        };
    }

    ////////////////////////////////////////////////////////////
    // Sorter

    struct CPPSORT_DEPRECATED("split_sorter is deprecated and will be removed in version 2.0.0, use split_adapter(pdq_sort) instead")
    split_sorter:
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

    CPPSORT_DEPRECATED("split_sort is deprecated and will be removed in version 2.0.0, use split_adapter(pdq_sort) instead")
    inline constexpr split_sorter split_sort{};
}

#endif // CPPSORT_SORTERS_SPLIT_SORTER_H_
