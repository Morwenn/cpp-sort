/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>

TEST_CASE( "miscellaneous tests for buffer providers",
           "[utility][buffer]" )
{
    using namespace cppsort;

    SECTION( "fixed_buffer" )
    {
        utility::fixed_buffer<128>::buffer<int> buffer;

        CHECK( buffer.size() == 128 );
        CHECK( buffer.begin() == buffer.cbegin() );
        CHECK( buffer.end() == buffer.cend() );
        CHECK( buffer.end() == buffer.begin() + buffer.size() );
    }

    SECTION( "fixed_buffer of size 0" )
    {
        utility::fixed_buffer<0>::buffer<int> buffer;

        CHECK( buffer.size() == 0 );
        CHECK( buffer.begin() == buffer.cbegin() );
        CHECK( buffer.end() == buffer.cend() );
        CHECK( buffer.end() == buffer.begin() + buffer.size() );
        CHECK( buffer.begin() == buffer.end() );
    }

    SECTION( "dynamic_buffer" )
    {
        utility::dynamic_buffer<utility::sqrt>::buffer<int> buffer(25);

        CHECK( buffer.size() == 5 );
        CHECK( buffer.begin() == buffer.cbegin() );
        CHECK( buffer.end() == buffer.cend() );
        CHECK( buffer.end() == buffer.begin() + buffer.size() );
    }
}
