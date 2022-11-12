/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <testing-tools/algorithm.h>

namespace
{
    template<typename Sorter>
    constexpr auto test_constexpr_sorter()
        -> bool
    {
        constexpr std::size_t size = 60;
        int collection[size] = {
            15, 6, 0, 2, 2, 3, 8, 12, 10, 5, 9, 7, 10, 45, 18,
            15, 6, 0, 2, 2, 3, 8, 12, 10, 5, 9, 7, 10, 45, 18,
            15, 6, 0, 2, 2, 3, 8, 12, 10, 5, 9, 7, 10, 45, 18,
            15, 6, 0, 2, 2, 3, 8, 12, 10, 5, 9, 7, 10, 45, 18,
        };
        Sorter sorter;
        sorter(collection, collection + size);
        return helpers::is_sorted(collection, collection + size);
    }
}

TEMPLATE_TEST_CASE( "test every constexpr sorter", "[sorters][constexpr]",
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<7>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::pdq_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    constexpr bool is_sorted = test_constexpr_sorter<TestType>();
    STATIC_CHECK( is_sorted );
}
