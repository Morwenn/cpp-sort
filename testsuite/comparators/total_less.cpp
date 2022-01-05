/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cmath>
#include <limits>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/total_less.h>
#include <cpp-sort/sorters/heap_sorter.h>

TEST_CASE( "IEEE 754 totalOrder implementation", "[comparison]" )
{
    static constexpr double nan = std::numeric_limits<double>::quiet_NaN();
    static constexpr double inf = std::numeric_limits<double>::infinity();

    double array[] = { +1.0, +inf, -1.0, -nan, +0.0, -inf, +nan, -0.0 };
    cppsort::heap_sort(array, cppsort::total_less);

    // Check for IEEE 754 totalOrder,
    // ignore quiet vs. signaling NaNs
    CHECK( std::isnan(array[0]) );
    CHECK( std::signbit(array[0]) );
    CHECK( std::isinf(array[1]) );
    CHECK( std::signbit(array[1]) );
    CHECK( array[2] == -1.0 );
    CHECK( array[3] == 0.0 );
    CHECK( std::signbit(array[3]) );
    CHECK( array[4] == 0.0 );
    CHECK( not std::signbit(array[4]) );
    CHECK( array[5] == +1.0 );
    CHECK( std::isinf(array[6]) );
    CHECK( not std::signbit(array[6]) );
    CHECK( std::isnan(array[7]) );
    CHECK( not std::signbit(array[7]) );
}
