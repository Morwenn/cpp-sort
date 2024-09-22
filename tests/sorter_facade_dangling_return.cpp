/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <concepts>
#include <functional>
#include <ranges>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/heap_sorter.h>

TEST_CASE( "sorter_facade returning dangling for rvalue ranges",
           "[sorter_facade][comparison][projection]" )
{
    SECTION( "range" )
    {
        auto it = cppsort::heap_sort(std::vector<int>{3, 2, 1});
        STATIC_CHECK( std::same_as<decltype(it), std::ranges::dangling> );
    }

    SECTION( "range and comparison" )
    {
        auto it = cppsort::heap_sort(std::vector<int>{3, 2, 1}, std::greater{});
        STATIC_CHECK( std::same_as<decltype(it), std::ranges::dangling> );
    }

    SECTION( "range and projection" )
    {
        auto it = cppsort::heap_sort(std::vector<int>{3, 2, 1}, std::negate{});
        STATIC_CHECK( std::same_as<decltype(it), std::ranges::dangling> );
    }

    SECTION( "range, comparison and projection" )
    {
        auto it = cppsort::heap_sort(std::vector<int>{3, 2, 1}, std::greater{}, std::negate{});
        STATIC_CHECK( std::same_as<decltype(it), std::ranges::dangling> );
    }
}