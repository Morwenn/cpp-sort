/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

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
