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
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/container_aware_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include "../distributions.h"

TEST_CASE( "container_aware_adapter and std::list",
           "[container_aware_adapter]" )
{
    // Tests for the sorters that have container-aware
    // overloads for std::list

    std::vector<double> vec; vec.reserve(187);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 187, -24.0);

    SECTION( "insertion_sorter" )
    {
        using sorter = cppsort::container_aware_adapter<
            cppsort::insertion_sorter
        >;
        std::list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        cppsort::sort(sorter{}, vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::container_aware_adapter<
            cppsort::merge_sorter
        >;
        std::list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        cppsort::sort(sorter{}, vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }

    SECTION( "selection_sorter" )
    {
        using sorter = cppsort::container_aware_adapter<
            cppsort::selection_sorter
        >;
        std::list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter{}(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        collection = { std::begin(vec), std::end(vec) };
        cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        cppsort::sort(sorter{}, vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }
}
