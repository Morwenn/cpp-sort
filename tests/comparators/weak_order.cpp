/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cmath>
#include <limits>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/weak_greater.h>
#include <cpp-sort/comparators/weak_less.h>
#include <cpp-sort/sorters/heap_sorter.h>

TEST_CASE( "Weak ordering of floating-point numbers", "[comparison]" )
{
    constexpr double inf = std::numeric_limits<double>::infinity();

    double array[] = { +1.0, +inf, -1.0, -std::nan("2"), +0.0, -inf, +std::nan("1"), -0.0 };

    SECTION( "weak_less" )
    {
        cppsort::heap_sort(array, cppsort::weak_less);

        // Check that equivalent values compare equivalent,
        // regardless of their representation
        CHECK( std::isnan(array[0]) );
        CHECK( std::signbit(array[0]) );
        CHECK( std::isinf(array[1]) );
        CHECK( std::signbit(array[1]) );
        CHECK( array[2] == -1.0 );
        CHECK( array[3] == 0.0 );
        CHECK( array[4] == 0.0 );
        CHECK( array[5] == +1.0 );
        CHECK( std::isinf(array[6]) );
        CHECK( not std::signbit(array[6]) );
        CHECK( std::isnan(array[7]) );
        CHECK( not std::signbit(array[7]) );
    }

    SECTION( "weak_greater" )
    {
        cppsort::heap_sort(array, cppsort::weak_greater);

        // Check that equivalent values compare equivalent,
        // regardless of their representation
        CHECK( not std::signbit(array[0]) );
        CHECK( std::isnan(array[0]) );
        CHECK( std::isinf(array[1]) );
        CHECK( not std::signbit(array[1]) );
        CHECK( array[2] == +1.0 );
        CHECK( array[3] == 0.0 );
        CHECK( array[4] == 0.0 );
        CHECK( array[5] == -1.0 );
        CHECK( std::isinf(array[6]) );
        CHECK( std::signbit(array[6]) );
        CHECK( std::isnan(array[7]) );
        CHECK( std::signbit(array[7]) );
    }
}
