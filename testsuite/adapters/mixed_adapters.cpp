/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 Morwenn
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
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/counting_adapter.h>
#include <cpp-sort/adapters/indirect_adapter.h>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include "../algorithm.h"

//
// Tests several adapters at once
//

TEST_CASE( "indirect sort with Schwartzian transform",
           "[indirect_adapter][schwartz_adapter]" )
{
    struct wrapper { short value; };

    std::vector<wrapper> collection(334);
    helpers::iota(std::begin(collection), std::end(collection), -93, &wrapper::value);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    SECTION( "schwartz_adapter over indirect_adapter" )
    {
        cppsort::schwartz_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over schwartz_adapter" )
    {
        cppsort::indirect_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "schwartz_adapter over schwartz_adapter" )
    {
        cppsort::schwartz_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over indirect_adapter" )
    {
        cppsort::indirect_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}

//
// Stability of mixed adapters without dedicated specializations
//

TEST_CASE( "stability of counting_adapter over self_sort_adapter",
           "[counting_adapter][self_sort_adapter][is_stable]" )
{
    // Big checks to ensure that mixed sorters have a valid stability

    using sorter1 = cppsort::counting_adapter<
        cppsort::self_sort_adapter<
            cppsort::selection_sorter
        >
    >;

    using sorter2 = cppsort::counting_adapter<
        cppsort::self_sort_adapter<
            cppsort::insertion_sorter
        >
    >;

    SECTION( "is_always_stable" )
    {
        CHECK( not cppsort::is_always_stable<sorter1>::value );
        CHECK( not cppsort::is_always_stable<sorter2>::value );
    }

    SECTION( "is_stable" )
    {
        CHECK( cppsort::is_stable<sorter1(std::list<int>&)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>&)>::value );
        CHECK( cppsort::is_stable<sorter1(std::list<int>&, std::greater<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>&, std::greater<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::list<int>&, std::negate<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>&, std::negate<>)>::value );

        CHECK( not cppsort::is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator, std::greater<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator, std::greater<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator, std::negate<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator, std::negate<>)>::value );

        CHECK( cppsort::is_stable<sorter2(std::list<int>&)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>&)>::value );
        CHECK( cppsort::is_stable<sorter2(std::list<int>&, std::greater<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>&, std::greater<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::list<int>&, std::negate<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>&, std::negate<>)>::value );

        CHECK( cppsort::is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator)>::value );
        CHECK( cppsort::is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator, std::greater<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator, std::greater<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator, std::negate<>)>::value );
        CHECK( cppsort::is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator, std::negate<>)>::value );
    }
}
