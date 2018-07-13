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
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "move_only.h"

TEST_CASE( "test every sorter with move-only types", "[sorters]" )
{
    // General test to make sure that every sorter compiles fine
    // and works fine with move-only types, additionally checking
    // that no read-after-move operation is performed

    std::vector<move_only<double>> collection;
    for (long i = -56.0 ; i < 366.0 ; ++i)
    {
        collection.emplace_back(i);
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "block_sorter" )
    {
        sort(cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "default_sorter" )
    {
        cppsort::sort(cppsort::default_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "drop_merge_sorter" )
    {
        cppsort::sort(cppsort::drop_merge_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        sort(cppsort::grail_sorter<cppsort::utility::fixed_buffer<0>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::sort(cppsort::heap_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        cppsort::sort(cppsort::insertion_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        cppsort::sort(cppsort::merge_insertion_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::sort(cppsort::merge_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::sort(cppsort::pdq_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::sort(cppsort::poplar_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_merge_sorter" )
    {
        cppsort::sort(cppsort::quick_merge_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::sort(cppsort::quick_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::sort(cppsort::selection_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::sort(cppsort::smooth_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        cppsort::sort(cppsort::std_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::sort(cppsort::tim_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::sort(cppsort::verge_sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
