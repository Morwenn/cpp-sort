/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <numeric>
#include <random>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/spread_sorter.h>

TEST_CASE( "spread_sorter tests", "[spread_sorter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    SECTION( "sort with int iterable" )
    {
        std::vector<int> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<unsigned> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0u);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with float iterable" )
    {
        std::vector<float> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0.0f);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<double> vec(100'000);
        std::iota(std::begin(vec), std::end(vec), 0.0);
        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(std::begin(vec), std::end(vec));
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }

    SECTION( "reverse sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        std::shuffle(std::begin(vec), std::end(vec), engine);
        cppsort::spread_sort(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }
}
