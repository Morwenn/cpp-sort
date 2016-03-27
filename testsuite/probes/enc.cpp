/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <forward_list>
#include <iterator>
#include <catch.hpp>
#include <cpp-sort/probes/enc.h>

TEST_CASE( "presortedness measure: enc", "[probe][enc]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 4, 6, 5, 2, 9, 1, 3, 8, 0, 7 };
        CHECK( cppsort::probe::enc(li) == 2 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 2 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::enc(li) == 0 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to half the size
        // of the input sequence minus one

        std::forward_list<int> li = { 0, 9, 1, 8, 2, 7, 3, 6, 4, 5 };
        CHECK( cppsort::probe::enc(li) == 4 );
        CHECK( cppsort::probe::enc(std::begin(li), std::end(li)) == 4 );
    }
}
