/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
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
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "../distributions.h"

TEST_CASE( "test sorter with descending_sawtooth distribution", "[distributions]" )
{
    std::vector<int> collection;
    collection.reserve(10'000);
    auto distribution = dist::descending_sawtooth{};
    distribution(std::back_inserter(collection), collection.size());

    SECTION( "block_sorter" )
    {
        using namespace cppsort;

        // Fixed buffer
        cppsort::sort(block_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Dynamic buffer
        cppsort::sort(block_sorter<utility::dynamic_buffer<utility::sqrt>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        using namespace cppsort;

        // Fixed buffer
        cppsort::sort(grail_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Dynamic buffer
        cppsort::sort(grail_sorter<utility::dynamic_buffer<utility::half>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::sort(cppsort::heap_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::sort(cppsort::merge_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::sort(cppsort::pdq_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::sort(cppsort::poplar_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::sort(cppsort::quick_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::sort(cppsort::smooth_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::sort(cppsort::spread_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        cppsort::sort(cppsort::std_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::sort(cppsort::tim_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::sort(cppsort::verge_sort, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
