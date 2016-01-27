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
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/probes/exc.h>

TEST_CASE( "presortedness measure: exc", "[probe][exc]" )
{
    SECTION( "simple test" )
    {
        std::vector<int> vec = { 74, 59, 62, 23, 86, 69, 18, 52, 77, 68 };
        CHECK( cppsort::probe::exc(vec) == 7 );
        CHECK( cppsort::probe::exc(std::begin(vec), std::end(vec)) == 7 );

        std::vector<int> vec2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( cppsort::probe::exc(vec2) == 5 );
        CHECK( cppsort::probe::exc(std::begin(vec2), std::end(vec2)) == 5 );
    }

    SECTION( "lower bound" )
    {
        std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::exc(vec) == 0 );
        CHECK( cppsort::probe::exc(std::begin(vec), std::end(vec)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        std::vector<int> vec = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( cppsort::probe::exc(vec) == 10 );
        CHECK( cppsort::probe::exc(std::begin(vec), std::end(vec)) == 10 );
    }
}
