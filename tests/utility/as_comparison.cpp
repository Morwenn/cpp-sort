/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <map>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/is_transparent.h>

TEST_CASE( "is_transparent over as_projection", "[utility][comparison][is_transparent]" )
{
    using cmp1 = cppsort::utility::detail::as_comparison_fn<std::less<is_transparent_helper_stored>>;
    using cmp2 = cppsort::utility::detail::as_comparison_fn<std::less<>>;

    STATIC_CHECK_FALSE( cppsort::detail::has_is_transparent_v<cmp1> );
    STATIC_CHECK( cppsort::detail::has_is_transparent_v<cmp2> );

    std::map<is_transparent_helper_stored, int, cmp1> mapping1 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_THROWS( mapping1.find(is_transparent_helper_compared(2)) );

    std::map<is_transparent_helper_stored, int, cmp2> mapping2 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_NOTHROW( mapping2.find(is_transparent_helper_compared(2)) );
}
