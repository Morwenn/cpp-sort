/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cmath>
#include <functional>
#include <iterator>
#include <limits>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/partial_greater.h>
#include <cpp-sort/comparators/partial_less.h>
#include <cpp-sort/sorters/heap_sorter.h>
#include <testing-tools/comparators.h>

TEST_CASE( "Partial ordering of floating-point numbers", "[comparison]" )
{
    constexpr double inf = std::numeric_limits<double>::infinity();

    // We can't do anything with NaNs in the partial order
    double array[] = { +1.0, +inf, -1.0, -2.3, +0.0, -inf, +5.6, -0.0, +inf, -inf };

    SECTION( "partial_less" )
    {
        cppsort::heap_sort(array, cppsort::partial_less);

        CHECK( std::isinf(array[0]) );
        CHECK( std::signbit(array[0]) );
        CHECK( std::isinf(array[1]) );
        CHECK( std::signbit(array[1]) );
        CHECK( std::isinf(array[8]) );
        CHECK( not std::signbit(array[8]) );
        CHECK( std::isinf(array[9]) );
        CHECK( not std::signbit(array[9]) );

        CHECK( std::is_sorted(std::begin(array), std::end(array)) );
    }

    SECTION( "weak_greater" )
    {
        cppsort::heap_sort(array, cppsort::partial_greater);

        CHECK( std::isinf(array[0]) );
        CHECK( not std::signbit(array[0]) );
        CHECK( std::isinf(array[1]) );
        CHECK( not std::signbit(array[1]) );
        CHECK( std::isinf(array[8]) );
        CHECK( std::signbit(array[8]) );
        CHECK( std::isinf(array[9]) );
        CHECK( std::signbit(array[9]) );

        CHECK( std::is_sorted(std::begin(array), std::end(array), std::greater<>{}) );
    }
}

TEST_CASE( "Partial order customization point", "[comparison]" )
{
    helpers::totally_comparable ta, tb;
    helpers::weakly_comparable wa, wb;
    helpers::partially_comparable pa, pb;

    // Ensure that overload resolution is correct
    STATIC_CHECK( cppsort::partial_less(ta, tb) == helpers::compare_result::total_less );
    STATIC_CHECK( cppsort::partial_greater(ta, tb) == helpers::compare_result::total_greater );
    STATIC_CHECK( cppsort::partial_less(wa, wb) == helpers::compare_result::weak_less );
    STATIC_CHECK( cppsort::partial_greater(wa, wb) == helpers::compare_result::weak_greater );
    STATIC_CHECK( cppsort::partial_less(pa, pb) == helpers::compare_result::partial_less );
    STATIC_CHECK( cppsort::partial_greater(pa, pb) == helpers::compare_result::partial_greater );
}
