/*
 * Copyright (c) 2017-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/no_post_iterator.h>

TEMPLATE_TEST_CASE( "test most sorters with no_post_iterator", "[sorters]",
                    cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>,
                    cppsort::counting_sorter,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<int> collection;
    for (int i = 56 ; i < 366 ; ++i) {
        collection.emplace_back(i);
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);

    // Iterators with no post-increment and no post-decrement
    auto first = make_no_post_iterator(std::begin(collection));
    auto last = make_no_post_iterator(std::end(collection));

    TestType sorter;
    sorter(first, last);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
}

TEMPLATE_TEST_CASE( "test type-specific sorters with no_post_iterator further",
                    "[sorters][ska_sorter][spread_sorter]",
                    cppsort::ska_sorter,
                    cppsort::spread_sorter )
{
    TestType sorter;

    std::vector<float> collection_float;
    for (float i = 56.0f ; i < 366.0f ; ++i) {
        collection_float.emplace_back(i);
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection_float), std::end(collection_float), engine);

    // Iterators with no post-increment and no post-decrement
    auto first_float = make_no_post_iterator(std::begin(collection_float));
    auto last_float = make_no_post_iterator(std::end(collection_float));

    sorter(first_float, last_float);
    CHECK( std::is_sorted(std::begin(collection_float), std::end(collection_float)) );

    std::vector<double> collection_double;
    for (double i = 56.0 ; i < 366.0 ; ++i) {
        collection_double.emplace_back(i);
    }
    std::shuffle(std::begin(collection_double), std::end(collection_double), engine);

    // Iterators with no post-increment and no post-decrement
    auto first_double = make_no_post_iterator(std::begin(collection_double));
    auto last_double = make_no_post_iterator(std::end(collection_double));

    sorter(first_double, last_double);
    CHECK( std::is_sorted(std::begin(collection_double), std::end(collection_double)) );

    std::vector<std::string> collection_str;
    for (long i = 56 ; i < 366 ; ++i) {
        collection_str.emplace_back(std::to_string(i));
    }
    std::shuffle(std::begin(collection_str), std::end(collection_str), engine);

    // Iterators with no post-increment and no post-decrement
    auto first_str = make_no_post_iterator(std::begin(collection_str));
    auto last_str = make_no_post_iterator(std::end(collection_str));

    sorter(first_str, last_str);
    CHECK( std::is_sorted(std::begin(collection_str), std::end(collection_str)) );
}
