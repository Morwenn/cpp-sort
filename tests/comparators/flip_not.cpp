/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/comparators/not_fn.h>

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
