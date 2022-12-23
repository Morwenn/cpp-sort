/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
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

    std::list<long long int> li(collection.begin(), collection.end());
    std::forward_list<long long int> fli(collection.begin(), collection.end());

    SECTION( "adaptive_shivers_sort" )
    {
        cppsort::adaptive_shivers_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "cartesian_tree_sort" )
    {
        cppsort::cartesian_tree_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "counting_sort" )
    {
        cppsort::counting_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "d_ary_heap_sort" )
    {
        cppsort::d_ary_heap_sort<5>(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "drop_merge_sort" )
    {
        cppsort::drop_merge_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "grail_sorter" )
    {
        cppsort::grail_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "heap_sorter" )
    {
        cppsort::heap_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "insertion_sorter" )
    {
        cppsort::insertion_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "mel_sorter" )
    {
        cppsort::mel_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        cppsort::merge_insertion_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::merge_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "pdq_sorter" )
    {
        cppsort::pdq_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "poplar_sorter" )
    {
        cppsort::poplar_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "quick_merge_sorter" )
    {
        cppsort::quick_merge_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );

        cppsort::quick_merge_sort(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );

        cppsort::quick_merge_sort(fli);
        CHECK( std::is_sorted(fli.begin(), fli.end()) );
    }

    SECTION( "quick_sorter" )
    {
        cppsort::quick_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::selection_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "ska_sorter" )
    {
        cppsort::ska_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "slab_sorter" )
    {
        cppsort::slab_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "smooth_sorter" )
    {
        cppsort::smooth_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "spin_sorter" )
    {
        cppsort::spin_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "splay_sorter" )
    {
        cppsort::splay_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "split_sorter" )
    {
        cppsort::split_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::spread_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "std_sorter" )
    {
        cppsort::std_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "tim_sorter" )
    {
        cppsort::tim_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "verge_sorter" )
    {
        cppsort::verge_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "wiki_sort" )
    {
        cppsort::wiki_sort(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}
