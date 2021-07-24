/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <array>
#include <iterator>
#include <numeric>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/utility/sorting_networks.h>
#include <testing-tools/distributions.h>

TEST_CASE( "sorting with index pairs", "[utility][sorting_networks]" )
{
    // Index pairs for a "brick" sorting network to sort 8 inputs
    constexpr std::array<cppsort::utility::index_pair<int>, 28> pairs = {{
        {0, 1}, {2, 3}, {4, 5}, {6, 7},
        {1, 2}, {3, 4}, {5, 6},
        {0, 1}, {2, 3}, {4, 5}, {6, 7},
        {1, 2}, {3, 4}, {5, 6},
        {0, 1}, {2, 3}, {4, 5}, {6, 7},
        {1, 2}, {3, 4}, {5, 6},
        {0, 1}, {2, 3}, {4, 5}, {6, 7},
        {1, 2}, {3, 4}, {5, 6},
    }};

    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 8);

    SECTION( "swap_index_pairs" )
    {
        swap_index_pairs(vec.begin(), pairs);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "swap_index_pairs_force_unroll" )
    {
        swap_index_pairs_force_unroll(vec.begin(), pairs);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}

TEST_CASE( "sorting with index pairs from sorting_network_sorter",
           "[utility][sorting_networks]" )
{
    constexpr auto pairs = cppsort::sorting_network_sorter<8>::index_pairs<int>();

    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 8);

    SECTION( "swap_index_pairs" )
    {
        swap_index_pairs(vec.begin(), pairs);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "swap_index_pairs_force_unroll" )
    {
        swap_index_pairs_force_unroll(vec.begin(), pairs);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}
