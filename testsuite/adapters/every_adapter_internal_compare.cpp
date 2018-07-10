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
#include <functional>
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include "../algorithm.h"
#include "../distributions.h"
#include "../internal_compare.h"

TEST_CASE( "test most adapters with a pointer to member function comparison",
           "[adapters][as_function]" )
{
    std::vector<internal_compare<int>> collection;
    collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    SECTION( "counting_adapter" )
    {
        using sorter = cppsort::counting_adapter<
            cppsort::selection_sorter
        >;

        // Sort and check it's sorted
        std::size_t res = sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "indirect_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "out_of_place_adapter" )
    {
        using sorter = cppsort::out_of_place_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        std::list<internal_compare<int>> li;
        distribution(std::back_inserter(li), 65, 0);

        sorter{}(li, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "schwartz_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        using sorter = cppsort::self_sort_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        std::list<internal_compare<int>> li;
        distribution(std::back_inserter(li), 65, 0);

        sorter{}(li, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::self_sort_adapter<cppsort::poplar_sorter>
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        std::list<internal_compare<int>> li;
        distribution(std::back_inserter(li), 65, 0);

        sorter{}(li, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
    }

    SECTION( "small_array_adapter" )
    {
        using namespace cppsort;

        std::array<internal_compare<int>, 6> arr = {{ {4}, {3}, {2}, {5}, {6}, {1} }};

        auto to_sort = arr;
        small_array_adapter<low_comparisons_sorter>{}(to_sort, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort)) );

        to_sort = arr;
        small_array_adapter<low_moves_sorter>{}(to_sort, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort)) );

        to_sort = arr;
        small_array_adapter<sorting_network_sorter>{}(to_sort, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort)) );
    }

    SECTION( "stable_adapter" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_adapter" )
    {
        using sorter = cppsort::verge_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(collection, &internal_compare<int>::compare_to);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
