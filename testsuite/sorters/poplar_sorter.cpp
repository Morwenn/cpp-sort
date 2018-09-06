/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017-2018 Morwenn
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
#include <cpp-sort/sorters/poplar_sorter.h>
#include "../distributions.h"

TEST_CASE( "poplar_sorter tests", "[poplar_sorter]" )
{
    // Distribution used to generate the data to sort
    auto distribution = dist::shuffled{};

    SECTION( "tricky test case 131073" )
    {
        // With signed 32-bit difference_type for an iterator,
        // sorting a collection of 131073 elements triggered a
        // problem with the underlying bit tricks that eventually
        // made the algorithm fail

        auto size = 131073; // This part matters the most
        std::vector<int> vec; vec.reserve(size);
        distribution(std::back_inserter(vec), size, -1568);
        cppsort::poplar_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }
}
