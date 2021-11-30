/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/spread_sorter.h>
#include <testing-tools/distributions.h>
#include <testing-tools/random.h>

TEST_CASE( "spread_sorter tests", "[spread_sorter]" )
{
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
        distribution.call<float>(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<double> vec;
        distribution.call<double>(std::back_inserter(vec), 100'000);
        cppsort::spread_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(vec.begin(), vec.end(), random::engine());
        cppsort::spread_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        std::shuffle(vec.begin(), vec.end(), random::engine());
        cppsort::spread_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "reverse sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(vec.begin(), vec.end(), random::engine());
        cppsort::spread_sort(vec, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        std::shuffle(vec.begin(), vec.end(), random::engine());
        cppsort::spread_sort(vec.begin(), vec.end(), std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }
}
