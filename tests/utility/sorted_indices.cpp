/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/heap_sorter.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/utility/sorted_indices.h>
#include <testing-tools/distributions.h>

TEST_CASE( "basic sorted_indices test", "[utility][sorted_indices]" )
{
    SECTION( "simple case" )
    {
        auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::heap_sorter>{};
        const std::vector<int> vec = { 6, 4, 2, 1, 8, 7, 0, 9, 5, 3 };
        auto indices = get_sorted_indices_for(vec);

        std::vector<std::ptrdiff_t> expected = { 6, 3, 2, 9, 1, 8, 0, 5, 4, 7 };
        CHECK( indices == expected );
    }

    SECTION( "empty collection" )
    {
        auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::heap_sorter>{};
        const std::vector<int> vec = {};
        auto indices = get_sorted_indices_for(vec);

        std::vector<std::ptrdiff_t> expected = {};
        CHECK( indices == expected );
    }

    SECTION( "all_equal" )
    {
        // Use a stable algorithm to get deterministic results for equivalent elements
        auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::insertion_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::all_equal{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_indices_for(vec);

        std::vector<std::ptrdiff_t> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( indices == expected );
    }

    SECTION( "ascending" )
    {
        auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::heap_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::ascending{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_indices_for(vec);

        std::vector<std::ptrdiff_t> expected = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( indices == expected );
    }

    SECTION( "descending" )
    {
        auto get_sorted_indices_for = cppsort::utility::sorted_indices<cppsort::heap_sorter>{};
        std::vector<int> vec;
        auto distribution = dist::descending{};
        distribution(std::back_inserter(vec), 10);
        auto indices = get_sorted_indices_for(vec);

        std::vector<std::ptrdiff_t> expected = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( indices == expected );
    }
}
