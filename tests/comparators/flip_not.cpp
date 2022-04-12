/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <map>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/comparators/not_fn.h>
#include <testing-tools/is_transparent.h>

TEST_CASE( "flip", "[comparison]" )
{
    SECTION( "flip" )
    {
        constexpr auto cmp = cppsort::flip(std::less<>{});
        STATIC_CHECK_FALSE( cmp(1, 2) );
        STATIC_CHECK( cmp(2, 1) );
        STATIC_CHECK_FALSE( cmp(2, 2) );
    }

    SECTION( "flip(flip)" )
    {
        constexpr auto cmp = cppsort::flip(cppsort::flip(std::less<>{}));
        STATIC_CHECK( cmp(1, 2) );
        STATIC_CHECK_FALSE( cmp(2, 1) );
        STATIC_CHECK_FALSE( cmp(2, 2) );
    }
}

TEST_CASE( "not_fn", "[comparison]" )
{
    SECTION( "not_fn" )
    {
        constexpr auto cmp = cppsort::not_fn(std::less<>{});
        STATIC_CHECK_FALSE( cmp(1, 2) );
        STATIC_CHECK( cmp(2, 1) );
        STATIC_CHECK( cmp(2, 2) );
    }

    SECTION( "not_fn(not_fn)" )
    {
        constexpr auto cmp = cppsort::not_fn(cppsort::not_fn(std::less<>{}));
        STATIC_CHECK( cmp(1, 2) );
        STATIC_CHECK_FALSE( cmp(2, 1) );
        STATIC_CHECK_FALSE( cmp(2, 2) );
    }
}

TEST_CASE( "mixed flip and not_fn", "[comparison]" )
{
    SECTION( "not_fn(flip)" )
    {
        constexpr auto cmp = cppsort::not_fn(cppsort::flip(std::less<>{}));
        STATIC_CHECK( cmp(1, 2) );
        STATIC_CHECK_FALSE( cmp(2, 1) );
        STATIC_CHECK( cmp(2, 2) );
    }

    SECTION( "flip(not_fn)" )
    {
        constexpr auto cmp = cppsort::flip(cppsort::not_fn(std::less<>{}));
        STATIC_CHECK( cmp(1, 2) );
        STATIC_CHECK_FALSE( cmp(2, 1) );
        STATIC_CHECK( cmp(2, 2) );
    }

    SECTION( "not_fn(flip(not_fn))" )
    {
        constexpr auto cmp = cppsort::not_fn(cppsort::flip(cppsort::not_fn(std::less<>{})));
        STATIC_CHECK_FALSE( cmp(1, 2) );
        STATIC_CHECK( cmp(2, 1) );
        STATIC_CHECK_FALSE( cmp(2, 2) );
    }

    SECTION( "flip(not_fn(flip))" )
    {
        constexpr auto cmp = cppsort::flip(cppsort::not_fn(cppsort::flip(std::less<>{})));
        STATIC_CHECK_FALSE( cmp(1, 2) );
        STATIC_CHECK( cmp(2, 1) );
        STATIC_CHECK( cmp(2, 2) );
    }
}

TEST_CASE( "is_transparent over flip", "[comparison][is_transparent]" )
{
    using cmp1 = cppsort::flip_t<std::less<is_transparent_helper_stored>>;
    using cmp2 = cppsort::flip_t<std::less<>>;

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

TEST_CASE( "is_transparent over not_fn", "[comparison][is_transparent]" )
{
    using cmp1 = cppsort::not_fn_t<std::less<is_transparent_helper_stored>>;
    using cmp2 = cppsort::not_fn_t<std::less<>>;

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
