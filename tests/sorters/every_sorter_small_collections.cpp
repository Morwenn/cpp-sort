/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>

TEMPLATE_TEST_CASE( "test every sorter with small collections", "[sorters]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<9>,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::ska_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<> )
{
    // Test that all sorters are able to sort empty collections or
    // collections with very few elements
    TestType sorter;

    SECTION( "empty collection" )
    {
        std::vector<int> collection;
        sorter(collection);
        SUCCEED();
    }

    SECTION( "one element" )
    {
        std::vector<int> collection = { 5 };
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "2 elements" )
    {
        std::vector<int> collection = { 8, 2 };
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "3 elements" )
    {
        std::vector<int> collection = { 5, 6, 3 };
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "5 elements" )
    {
        std::vector<int> collection = { 8, 0, 1, 6, 3 };
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "10 element" )
    {
        std::vector<int> collection = { 5, 6, 7, 1, 2, 9, 3, 8, 0, 4 };
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}
