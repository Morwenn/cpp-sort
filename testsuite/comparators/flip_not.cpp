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
        auto cmp = cppsort::flip(std::less<>{});
        CHECK_FALSE( cmp(1, 2) );
        CHECK( cmp(2, 1) );
        CHECK_FALSE( cmp(2, 2) );
    }

    SECTION( "flip(flip)" )
    {
        auto cmp = cppsort::flip(cppsort::flip(std::less<>{}));
        CHECK( cmp(1, 2) );
        CHECK_FALSE( cmp(2, 1) );
        CHECK_FALSE( cmp(2, 2) );
    }
}

TEST_CASE( "not_fn", "[comparison]" )
{
    SECTION( "not_fn" )
    {
        auto cmp = cppsort::not_fn(std::less<>{});
        CHECK_FALSE( cmp(1, 2) );
        CHECK( cmp(2, 1) );
        CHECK( cmp(2, 2) );
    }

    SECTION( "not_fn(not_fn)" )
    {
        auto cmp = cppsort::not_fn(cppsort::not_fn(std::less<>{}));
        CHECK( cmp(1, 2) );
        CHECK_FALSE( cmp(2, 1) );
        CHECK_FALSE( cmp(2, 2) );
    }
}

TEST_CASE( "mixed flip and not_fn", "[comparison]" )
{
    SECTION( "not_fn(flip)" )
    {
        auto cmp = cppsort::not_fn(cppsort::flip(std::less<>{}));
        CHECK( cmp(1, 2) );
        CHECK_FALSE( cmp(2, 1) );
        CHECK( cmp(2, 2) );
    }

    SECTION( "flip(not_fn)" )
    {
        auto cmp = cppsort::flip(cppsort::not_fn(std::less<>{}));
        CHECK( cmp(1, 2) );
        CHECK_FALSE( cmp(2, 1) );
        CHECK( cmp(2, 2) );
    }

    SECTION( "not_fn(flip(not_fn))" )
    {
        auto cmp = cppsort::not_fn(cppsort::flip(cppsort::not_fn(std::less<>{})));
        CHECK_FALSE( cmp(1, 2) );
        CHECK( cmp(2, 1) );
        CHECK_FALSE( cmp(2, 2) );
    }

    SECTION( "flip(not_fn(flip))" )
    {
        auto cmp = cppsort::flip(cppsort::not_fn(cppsort::flip(std::less<>{})));
        CHECK_FALSE( cmp(1, 2) );
        CHECK( cmp(2, 1) );
        CHECK( cmp(2, 2) );
    }
}
