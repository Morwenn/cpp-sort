/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/spread_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/random.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "spread_sorter tests with projections",
           "[spread_sorter][projection]" )
{
    SECTION( "sort with int iterable" )
    {
        std::vector<std::pair<int, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, &std::pair<int, float>::first);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::less<>{}, &std::pair<int, float>::second) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<std::pair<unsigned, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, &std::pair<unsigned, float>::first);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::less<>{}, &std::pair<unsigned, float>::second) );
    }

    SECTION( "sort with float iterable" )
    {
        std::vector<std::pair<int, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, &std::pair<int, float>::second);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::less<>{}, &std::pair<int, float>::first) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<std::pair<int, double>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, double(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, &std::pair<int, double>::second);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::less<>{}, &std::pair<int, double>::first) );
    }

    SECTION( "sort with std::string iterators" )
    {
        using wrapper = generic_wrapper<std::string>;

        std::vector<wrapper> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(std::to_string(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "reverse sort with std::string iterators" )
    {
        using wrapper = generic_wrapper<std::string>;

        std::vector<wrapper> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(std::to_string(i));
        }
        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::spread_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(),
                                  std::greater<>{}, &wrapper::value) );
    }
}
