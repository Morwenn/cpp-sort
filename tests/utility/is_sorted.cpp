/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/utility/is_sorted.h>
#include <cpp-sort/utility/is_sorted_until.h>
#include <testing-tools/distributions.h>

namespace utility = cppsort::utility;

TEST_CASE( "is_sorted and is_sorted_until test", "[utility][is_sorted]" )
{
    SECTION( "empty collection" )
    {
        std::vector<int> vec = {};
        CHECK( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == vec.end() );
    }

    SECTION( "collection with one element" )
    {
        std::vector<int> vec = {5};
        CHECK( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == vec.end() );
    }

    SECTION( "collection with two elements" )
    {
        std::vector<int> vec1 = {0, 0};
        CHECK( utility::is_sorted(vec1) );
        CHECK( utility::is_sorted_until(vec1) == vec1.end() );

        std::vector<int> vec2 = {0, 5};
        CHECK( utility::is_sorted(vec2) );
        CHECK( utility::is_sorted_until(vec2) == vec2.end() );

        std::vector<int> vec3 = {5, 0};
        CHECK_FALSE( utility::is_sorted(vec3) );
        CHECK( utility::is_sorted_until(vec3) == std::next(vec3.begin()) );
    }

    SECTION( "distribution: ascending" )
    {
        std::vector<int> vec;
        vec.reserve(250);
        auto distribution = dist::ascending{};
        distribution(std::back_inserter(vec), 250);
        CHECK( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == vec.end() );
    }

    SECTION( "distribution: ascending_duplicates" )
    {
        std::vector<int> vec;
        vec.reserve(250);
        auto distribution = dist::ascending_duplicates{};
        distribution(std::back_inserter(vec), 250);
        CHECK( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == vec.end() );
    }

    SECTION( "distribution: all_equal" )
    {
        std::vector<int> vec;
        vec.reserve(250);
        auto distribution = dist::all_equal{};
        distribution(std::back_inserter(vec), 250);
        CHECK( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == vec.end() );
    }

    SECTION( "distribution: descending" )
    {
        std::vector<int> vec;
        vec.reserve(250);
        auto distribution = dist::descending{};
        distribution(std::back_inserter(vec), 250);
        CHECK_FALSE( utility::is_sorted(vec) );
        CHECK( utility::is_sorted_until(vec) == std::next(vec.begin()) );
    }
}
