/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Morwenn
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
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/spin_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "spin_sorter tests", "[spin_sorter]" )
{
    // spin_sorter has different paths that can only be reached depending
    // on the size of the input that generic tests don't seem to catch,
    // hence those additional tests. The different paths depend on the
    // following criteria:
    // - Whether log2(size(input)) is even or odd
    // - TODO

    std::vector<int> collection;
    auto distribution = dist::shuffled{};

    SECTION( "even number of levels" )
    {
        const int size = 2800;
        collection.reserve(size);
        distribution(std::back_inserter(collection), size, 0);
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "odd number of levels" )
    {
        const int size = 2300;
        collection.reserve(size);
        distribution(std::back_inserter(collection), size, 0);
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
