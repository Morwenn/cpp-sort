/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/utility/apply_permutation.h>
#include <cpp-sort/utility/sorted_indices.h>
#include <testing-tools/distributions.h>

TEST_CASE( "apply_permutation test", "[utility][apply_permutation]" )
{
    SECTION( "simple case" )
    {
        std::vector<int> vec = { 6, 4, 2, 1, 8, 7, 0, 9, 5, 3 };
        std::vector<std::ptrdiff_t> indices = { 6, 3, 2, 9, 1, 8, 0, 5, 4, 7 };
        cppsort::utility::apply_permutation(vec, indices);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "empty collection" )
    {
        std::vector<int> vec = {};
        std::vector<std::ptrdiff_t> indices = {};
        cppsort::utility::apply_permutation(vec, indices);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "ascending" )
    {
        std::vector<int> vec;
        auto distribution = dist::ascending{};
        distribution(std::back_inserter(vec), 10);
        std::vector<std::ptrdiff_t> indices = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        cppsort::utility::apply_permutation(vec, indices);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "descending" )
    {
        std::vector<int> vec;
        auto distribution = dist::descending{};
        distribution(std::back_inserter(vec), 10);
        std::vector<std::ptrdiff_t> indices = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        cppsort::utility::apply_permutation(vec, indices);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "big collection" )
    {
        std::vector<int> vec;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(vec), 1'000);
        auto get_sorted_indices_for = cppsort::utility::sorted_indices(cppsort::poplar_sort);
        std::vector<std::ptrdiff_t> indices = get_sorted_indices_for(vec);
        cppsort::utility::apply_permutation(vec, indices);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}
