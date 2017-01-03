/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include "../distributions.h"

TEST_CASE( "every sorter with indirect adapter",
           "[indirect_adapter]" )
{
    std::vector<double> collection; collection.reserve(412);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 412, -125.0);

    SECTION( "block_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::block_sorter<>>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "default_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::default_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::grail_sorter<>>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::heap_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::insertion_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::merge_insertion_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::merge_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::pdq_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::poplar_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::quick_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::selection_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::smooth_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::std_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "stable_adapter<std_sorter>" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::stable_adapter<cppsort::std_sorter>
        >;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::tim_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        using sorter = cppsort::indirect_adapter<cppsort::verge_sorter>;
        cppsort::sort(sorter{}, collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
