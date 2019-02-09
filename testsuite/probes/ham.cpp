/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2019 Morwenn
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
#include <catch2/catch.hpp>
#include <cpp-sort/probes/ham.h>
#include "../distributions.h"
#include "../internal_compare.h"

TEST_CASE( "presortedness measure: ham", "[probe][ham]" )
{
    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 34, 43, 96, 42, 44, 48, 57, 42, 68, 69 };
        CHECK( cppsort::probe::ham(li) == 6 );
        CHECK( cppsort::probe::ham(std::begin(li), std::end(li)) == 6 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( cppsort::probe::ham(tricky, &internal_compare<int>::compare_to) == 6 );
    }

    SECTION( "lower bound" )
    {
        std::forward_list<int> li = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        CHECK( cppsort::probe::ham(li) == 0 );
        CHECK( cppsort::probe::ham(std::begin(li), std::end(li)) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence

        std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        CHECK( cppsort::probe::ham(li) == 11 );
        CHECK( cppsort::probe::ham(std::begin(li), std::end(li)) == 11 );
    }

    SECTION( "regressions" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_sawtooth{};
        distribution(std::back_inserter(collection), 100);

        std::sort(std::begin(collection), std::end(collection));
        CHECK( cppsort::probe::ham(collection) == 0 );
    }
}
