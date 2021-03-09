/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/container_aware_adapter.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/mel_sorter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "container_aware_adapter and std::forward_list",
           "[container_aware_adapter]" )
{
    // Tests for the sorters that have container-aware
    // overloads for std::forward_list

    std::vector<double> vec; vec.reserve(187);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 187, -24.0);

    SECTION( "insertion_sorter" )
    {
        cppsort::container_aware_adapter<
            cppsort::insertion_sorter
        > sorter;
        std::forward_list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        sorter(vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }

    SECTION( "merge_sorter" )
    {
        cppsort::container_aware_adapter<
            cppsort::merge_sorter
        > sorter;
        std::forward_list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        sorter(vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }

    SECTION( "mel_sorter" )
    {
        cppsort::container_aware_adapter<
            cppsort::mel_sorter
        > sorter;
        std::forward_list<double> collection(vec.begin(), vec.end());

        collection = std::forward_list<double>(vec.begin(), vec.end());
        sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        collection = std::forward_list<double>(vec.begin(), vec.end());
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        collection = std::forward_list<double>(vec.begin(), vec.end());
        sorter(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end()) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        sorter(vec_copy);
        CHECK( std::is_sorted(vec_copy.begin(), vec_copy.end()) );
    }

    SECTION( "selection_sorter" )
    {
        cppsort::container_aware_adapter<
            cppsort::selection_sorter
        > sorter;
        std::forward_list<double> collection(std::begin(vec), std::end(vec));

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        collection = { std::begin(vec), std::end(vec) };
        sorter(collection, std::greater<>{}, std::negate<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );

        // Make sure that the generic overload is also called when needed

        auto vec_copy = vec;
        sorter(vec_copy);
        CHECK( std::is_sorted(std::begin(vec_copy), std::end(vec_copy)) );
    }
}
