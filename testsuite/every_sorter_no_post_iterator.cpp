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
#include <iterator>
#include <random>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "no_post_iterator.h"

TEST_CASE( "test every sorter with no_post_iterator", "[sorters]" )
{
    std::vector<int> collection;
    for (long i = 56 ; i < 366 ; ++i) {
        collection.emplace_back(i);
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    // Iterators with no post-increment and no post-decrement
    auto first = make_no_post_iterator(std::begin(collection));
    auto last = make_no_post_iterator(std::end(collection));

    SECTION( "block_sorter" )
    {
        sort(cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "counting_sorter" )
    {
        cppsort::sort(cppsort::counting_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "default_sorter" )
    {
        cppsort::sort(cppsort::default_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "drop_merge_sorter" )
    {
        cppsort::sort(cppsort::drop_merge_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        sort(cppsort::grail_sorter<cppsort::utility::fixed_buffer<0>>{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::sort(cppsort::heap_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        cppsort::sort(cppsort::insertion_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        cppsort::sort(cppsort::merge_insertion_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::sort(cppsort::merge_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::sort(cppsort::pdq_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::sort(cppsort::poplar_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_merge_sorter" )
    {
        cppsort::sort(cppsort::quick_merge_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::sort(cppsort::quick_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::sort(cppsort::selection_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "ska_sorter" )
    {
        cppsort::sort(cppsort::ska_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        std::vector<float> collection_float;
        for (long i = 56.0f ; i < 366.0f ; ++i) {
            collection_float.emplace_back(i);
        }
        std::shuffle(std::begin(collection_float), std::end(collection_float), engine);

        // Iterators with no post-increment and no post-decrement
        auto first_float = make_no_post_iterator(std::begin(collection_float));
        auto last_float = make_no_post_iterator(std::end(collection_float));

        cppsort::sort(cppsort::ska_sorter{}, first_float, last_float);
        CHECK( std::is_sorted(std::begin(collection_float), std::end(collection_float)) );

        std::vector<double> collection_double;
        for (long i = 56.0 ; i < 366.0 ; ++i) {
            collection_double.emplace_back(i);
        }
        std::shuffle(std::begin(collection_double), std::end(collection_double), engine);

        // Iterators with no post-increment and no post-decrement
        auto first_double = make_no_post_iterator(std::begin(collection_double));
        auto last_double = make_no_post_iterator(std::end(collection_double));

        cppsort::sort(cppsort::ska_sorter{}, first_double, last_double);
        CHECK( std::is_sorted(std::begin(collection_double), std::end(collection_double)) );

        std::vector<std::string> collection_str;
        for (long i = 56 ; i < 366 ; ++i) {
            collection_str.emplace_back(std::to_string(i));
        }
        std::shuffle(std::begin(collection_str), std::end(collection_str), engine);

        // Iterators with no post-increment and no post-decrement
        auto first_str = make_no_post_iterator(std::begin(collection_str));
        auto last_str = make_no_post_iterator(std::end(collection_str));

        cppsort::sort(cppsort::ska_sorter{}, first_str, last_str);
        CHECK( std::is_sorted(std::begin(collection_str), std::end(collection_str)) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::sort(cppsort::smooth_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::sort(cppsort::spread_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        std::vector<float> collection_float;
        for (long i = 56.0f ; i < 366.0f ; ++i) {
            collection_float.emplace_back(i);
        }
        std::shuffle(std::begin(collection_float), std::end(collection_float), engine);

        auto first_float = make_no_post_iterator(std::begin(collection_float));
        auto last_float = make_no_post_iterator(std::end(collection_float));

        cppsort::sort(cppsort::spread_sorter{}, first_float, last_float);
        CHECK( std::is_sorted(std::begin(collection_float), std::end(collection_float)) );

        std::vector<double> collection_double;
        for (long i = 56.0 ; i < 366.0 ; ++i) {
            collection_double.emplace_back(i);
        }
        std::shuffle(std::begin(collection_double), std::end(collection_double), engine);

        auto first_double = make_no_post_iterator(std::begin(collection_double));
        auto last_double = make_no_post_iterator(std::end(collection_double));

        cppsort::sort(cppsort::spread_sorter{}, first_double, last_double);
        CHECK( std::is_sorted(std::begin(collection_double), std::end(collection_double)) );

        std::vector<std::string> collection_str;
        for (long i = 56 ; i < 366 ; ++i) {
            collection_str.emplace_back(std::to_string(i));
        }
        std::shuffle(std::begin(collection_str), std::end(collection_str), engine);

        // Iterators with no post-increment and no post-decrement
        auto first_str = make_no_post_iterator(std::begin(collection_str));
        auto last_str = make_no_post_iterator(std::end(collection_str));

        cppsort::sort(cppsort::spread_sorter{}, first_str, last_str);
        CHECK( std::is_sorted(std::begin(collection_str), std::end(collection_str)) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::sort(cppsort::tim_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::sort(cppsort::verge_sorter{}, first, last);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
