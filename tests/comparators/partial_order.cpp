/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/partial_greater.h>
#include <cpp-sort/comparators/partial_less.h>
#include <testing-tools/comparators.h>

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
