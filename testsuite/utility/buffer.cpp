/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <catch2/catch.hpp>
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
