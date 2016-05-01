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
#include <algorithm>
#include <ctime>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "span.h"

TEST_CASE( "test every sorter with temporary span",
           "[sorters][span]" )
{
    // General test to make sure that every sorter compiles fine
    // and is able to sort a vector of numbers. spread_sorter is
    // already tested in-depth somewhere else and needs specific
    // tests, so it's not included here.

    std::vector<double> collection(491);
    std::iota(std::begin(collection), std::end(collection), -125);
    std::mt19937 engine(std::time(nullptr));
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "block_sorter" )
    {
        using namespace cppsort;

        // Fixed buffer
        sort(block_sorter<>{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Dynamic buffer
        sort(block_sorter<utility::dynamic_buffer<utility::sqrt>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "default_sorter" )
    {
        cppsort::sort(cppsort::default_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        using namespace cppsort;

        // Fixed buffer
        sort(grail_sorter<>{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Dynamic buffer
        sort(grail_sorter<utility::dynamic_buffer<utility::half>>{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::sort(cppsort::heap_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        cppsort::sort(cppsort::insertion_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        cppsort::sort(cppsort::merge_insertion_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::sort(cppsort::merge_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::sort(cppsort::pdq_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::sort(cppsort::poplar_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::sort(cppsort::quick_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::sort(cppsort::selection_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::sort(cppsort::smooth_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        cppsort::sort(cppsort::std_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::sort(cppsort::tim_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::sort(cppsort::verge_sorter{}, make_span(collection));
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
