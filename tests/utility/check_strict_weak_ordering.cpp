/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cmath>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/utility/check_strict_weak_ordering.h>

TEST_CASE( "check_strict_weak_ordering test", "[utility][check_strict_weak_ordering]" )
{
    using cppsort::utility::check_strict_weak_ordering;

    SECTION( "empty collection" )
    {
        std::vector<int> vec = {};
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "one element" )
    {
        std::vector<int> vec = { 0 };
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "one element, NaN" )
    {
        std::vector<double> vec = { std::nan("1") };
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "empty collection" )
    {
        std::vector<int> vec = {};
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "two elements" )
    {
        std::vector<int> vec1 = { 0, 0 };
        CHECK( check_strict_weak_ordering(vec1) );

        std::vector<int> vec2 = { 0, 5 };
        CHECK( check_strict_weak_ordering(vec2) );

        std::vector<int> vec3 = { 5, 0 };
        CHECK( check_strict_weak_ordering(vec3) );
    }

    SECTION( "two elements, NaN" )
    {
        std::vector<double> vec1 = { std::nan("1"), std::nan("1") };
        CHECK( check_strict_weak_ordering(vec1) );

        std::vector<double> vec2 = { std::nan("1"), 5 };
        CHECK( check_strict_weak_ordering(vec2) );

        std::vector<double> vec3 = { 5, std::nan("1") };
        CHECK( check_strict_weak_ordering(vec3) );
    }

    SECTION( "small collection" )
    {
        std::vector<int> vec = { 1, 4, 32, 5, 89, 43, 56, 8, 7, 2, 44, 37, 73 };
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "small collection with duplicates" )
    {
        std::vector<int> vec = { 1, 4, 32, 5, 1, 89, 43, 56, 8, 7, 2, 2, 4, 44, 37, 73 };
        CHECK( check_strict_weak_ordering(vec) );
    }

    SECTION( "small collection with NaN" )
    {
        std::vector<double> vec = {
            1.0, 4.0, 32.0, 5.0, 89.0, 43.0, 56.0, 345.0,
            8.0, 7.0, 2.0, std::nan("2"), 44.0, 37.0, 73.0,
        };
        CHECK_FALSE( check_strict_weak_ordering(vec) );
    }

    SECTION( "small collection with std::less_equal" )
    {
        std::vector<int> vec = { 1, 4, 32, 5, 89, 43, 56, 8, 7, 2, 44, 37, 73 };
        CHECK_FALSE( check_strict_weak_ordering(vec, std::less_equal{}) );
    }

    SECTION( "small collection with duplicates with std::less_equal" )
    {
        std::vector<int> vec = { 1, 4, 32, 5, 1, 89, 43, 56, 8, 7, 2, 2, 4, 44, 37, 73 };
        CHECK_FALSE( check_strict_weak_ordering(vec, std::less_equal{}) );
    }
}
