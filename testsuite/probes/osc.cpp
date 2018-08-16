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
#include <forward_list>
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/probes/osc.h>
#include "../internal_compare.h"

TEST_CASE( "presortedness measure: osc", "[probe][osc]" )
{
    SECTION( "simple test" )
    {
        // Example from the paper Adaptative Heapsort
        // by Levcopoulos and Petersson

        std::forward_list<int> li = { 6, 3, 9, 8, 4, 7, 1, 11 };
        CHECK( cppsort::probe::osc(li) == 17 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 17 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::osc(tricky, &internal_compare<int>::compare_to) == 17 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::osc(li) == 0 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // Example from the paper Adaptative Heapsort
        // by Levcopoulos and Petersson, the upper bound
        // should be (size * (size - 2) - 1) / 2

        std::forward_list<int> li = { 8, 5, 10, 3, 12, 1, 13, 2, 11, 4, 9, 6, 7 };
        CHECK( cppsort::probe::osc(li) == 71 );
        CHECK( cppsort::probe::osc(std::begin(li), std::end(li)) == 71 );
    }
}
