/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#include <catch2/catch.hpp>
#include <cpp-sort/probes/par.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "presortedness measure: par", "[probe][par]" )
{
    SECTION( "simple test" )
    {
        const std::vector<int> vec = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( cppsort::probe::par(vec) == 7 );
        CHECK( cppsort::probe::par(std::begin(vec), std::end(vec)) == 7 );

        std::vector<internal_compare<int>> tricky(vec.begin(), vec.end());
        CHECK( cppsort::probe::par(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "lower bound" )
    {
        const std::vector<int> vec = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::par(vec) == 0 );
        CHECK( cppsort::probe::par(std::begin(vec), std::end(vec)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::vector<int> vec = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
        CHECK( cppsort::probe::par(vec) == 10 );
        CHECK( cppsort::probe::par(std::begin(vec), std::end(vec)) == 10 );
    }
}
