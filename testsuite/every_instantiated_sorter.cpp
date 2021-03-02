/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <testing-tools/distributions.h>

TEST_CASE( "test every instantiated sorter", "[sorters]" )
{
    // Sorters are great, but if we can hide the abstraction
    // under instantiated sorters that look like regular
    // functions, then it's even better, but we need to test
    // them to, just to be sure
    //
    // Only default_sorter doesn't have a standard instance
    // since it is already used by default by cppsort::sort

    std::vector<long long int> collection; collection.reserve(35);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 35, -47);

    std::list<long long int> li(std::begin(collection), std::end(collection));
    std::forward_list<long long int> fli(std::begin(collection), std::end(collection));

    SECTION( "cartesian_tree_sort" )
    {
        cppsort::cartesian_tree_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "block_sort" )
    {
        cppsort::block_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "counting_sort" )
    {
        cppsort::counting_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "drop_merge_sort" )
    {
        cppsort::drop_merge_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        cppsort::grail_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::heap_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        cppsort::insertion_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        cppsort::merge_insertion_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::merge_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::pdq_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::poplar_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_merge_sorter" )
    {
        cppsort::quick_merge_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        cppsort::quick_merge_sort(li);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );

        cppsort::quick_merge_sort(fli);
        CHECK( std::is_sorted(std::begin(fli), std::end(fli)) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::quick_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::selection_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "ska_sorter" )
    {
        cppsort::ska_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::smooth_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "spin_sorter" )
    {
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "split_sorter" )
    {
        cppsort::split_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::spread_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        cppsort::std_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::tim_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::verge_sort(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
