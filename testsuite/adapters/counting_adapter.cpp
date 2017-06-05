/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2017 Morwenn
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
#include <cstddef>
#include <iterator>
#include <list>
#include <numeric>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/counting_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/sorters/std_sorter.h>
#include "../algorithm.h"
#include "../distributions.h"
#include "../span.h"

TEST_CASE( "basic counting_adapter tests",
           "[counting_adapter][selection_sorter]" )
{
    // Selection sort always makes the same number of comparisons
    // for a given size of arrays, allowing to deterministically
    // check that number of comparisons
    using sorter = cppsort::counting_adapter<
        cppsort::selection_sorter
    >;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        std::size_t res = cppsort::sort(sorter{}, collection);
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "with projections" )
    {
        struct wrapper { int value; };

        // Pseudo-random number engine
        std::mt19937_64 engine(Catch::rngSeed());

        // Fill the collection
        std::vector<wrapper> tmp(80);
        helpers::iota(std::begin(tmp), std::end(tmp), 0, &wrapper::value);
        std::shuffle(std::begin(tmp), std::end(tmp), engine);
        std::list<wrapper> collection(std::begin(tmp), std::end(tmp));

        // Sort and check it's sorted
        std::size_t res = cppsort::sort(sorter{}, collection, &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "counting_adapter tests with std_sorter",
           "[counting_adapter][std_sorter]" )
{
    using sorter = cppsort::counting_adapter<
        cppsort::std_sorter
    >;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::vector<int> collection; collection.reserve(65);
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}

TEST_CASE( "counting_adapter with span",
           "[counting_adapter][span][selection_sorter]" )
{
    using sorter = cppsort::counting_adapter<
        cppsort::selection_sorter
    >;

    SECTION( "without projections" )
    {
        // Fill the collection
        std::list<int> collection;
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(collection), 65, 0);

        // Sort and check it's sorted
        std::size_t res = cppsort::sort(sorter{}, make_span(collection));
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "with projections" )
    {
        struct wrapper { int value; };

        // Pseudo-random number engine
        std::mt19937_64 engine(Catch::rngSeed());

        // Fill the collection
        std::vector<wrapper> tmp(80);
        helpers::iota(std::begin(tmp), std::end(tmp), 0, &wrapper::value);
        std::shuffle(std::begin(tmp), std::end(tmp), engine);
        std::list<wrapper> collection(std::begin(tmp), std::end(tmp));

        // Sort and check it's sorted
        std::size_t res = cppsort::sort(sorter{}, make_span(collection), &wrapper::value);
        CHECK( res == 3160 );
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}
