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
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include "distributions.h"
#include "memory_exhaustion.h"

//
// Specific tests to check that a selection of sorters still run fine
// even when there isn't any extra heap memory available
//
// These tests shouldn't be part of the main test suite executable
//

TEMPLATE_TEST_CASE( "test heap exhaustion for random-access sorters", "[sorters][heap_exhaustion]",
                    cppsort::block_sorter<>,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "test heap exhaustion for bidirectional sorters", "[sorters][heap_exhaustion]",
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "test heap exhaustion for forward sorters", "[sorters][heap_exhaustion]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 491, -125);

    using sorter = TestType;
    {
        scoped_memory_exhaustion _;
        sorter{}(collection);
    }
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}
