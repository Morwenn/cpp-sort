/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <random>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/spread_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "spread_sorter tests", "[spread_sorter]" )
{
    // Pseudo-random number engine
    std::mt19937_64 engine(Catch::rngSeed());

    auto distribution = dist::shuffled{};

    SECTION( "sort with int iterable" )
    {
        std::vector<int> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<unsigned> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with float iterable" )
    {
        std::vector<float> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<double> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(vec.begin(), vec.end(), engine);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        std::shuffle(vec.begin(), vec.end(), engine);
        cppsort::spread_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "reverse sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(vec.begin(), vec.end(), engine);
        cppsort::spread_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        std::shuffle(vec.begin(), vec.end(), engine);
        cppsort::spread_sort(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }
}
