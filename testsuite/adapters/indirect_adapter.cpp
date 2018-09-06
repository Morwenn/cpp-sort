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
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/quick_sorter.h>
#include "../algorithm.h"
#include "../distributions.h"
#include "../span.h"

TEST_CASE( "basic tests with indirect_adapter",
           "[indirect_adapter]" )
{
    std::vector<int> vec; vec.reserve(221);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 221, -32);

    // Working shuffled copy
    auto collection = vec;

    using sorter = cppsort::indirect_adapter<
        cppsort::quick_sorter
    >;

    SECTION( "with comparison" )
    {
        cppsort::sort(sorter{}, collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = vec;
        cppsort::sort(sorter{}, std::begin(collection), std::end(collection), std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "with projection" )
    {
        cppsort::sort(sorter{}, collection, std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );

        collection = vec;
        cppsort::sort(sorter{}, std::begin(collection), std::end(collection), std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );
    }

    SECTION( "with comparison and projection" )
    {
        cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = vec;
        cppsort::sort(sorter{}, std::begin(collection), std::end(collection),
                      std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}

TEST_CASE( "indirect_adapter with temporary span",
           "[indirect_adapter][span]" )
{
    std::vector<int> collection; collection.reserve(221);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 221, -32);

    using sorter = cppsort::indirect_adapter<
        cppsort::quick_sorter
    >;

    SECTION( "with comparison" )
    {
        cppsort::sort(sorter{}, make_span(collection), std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "with projection" )
    {
        cppsort::sort(sorter{}, make_span(collection), std::negate<>{});
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, std::negate<>{}) );
    }

    SECTION( "with comparison and projection" )
    {
        cppsort::sort(sorter{}, make_span(collection), std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
