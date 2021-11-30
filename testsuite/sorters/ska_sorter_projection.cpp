/*
 * Copyright (c) 2017-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters/ska_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/random.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "ska_sorter tests with projections",
           "[ska_sorter][projection]" )
{
    SECTION( "sort with int iterable" )
    {
        std::vector<std::pair<int, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(std::begin(vec), std::end(vec), random::engine());
        cppsort::ska_sort(vec, &std::pair<int, float>::first);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec),
                                  std::less<>{}, &std::pair<int, float>::second) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<std::pair<unsigned, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(std::begin(vec), std::end(vec), random::engine());
        cppsort::ska_sort(vec, &std::pair<unsigned, float>::first);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec),
                                  std::less<>{}, &std::pair<unsigned, float>::second) );
    }

    SECTION( "sort with float iterable" )
    {
        std::vector<std::pair<int, float>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, float(i));
        }
        std::shuffle(std::begin(vec), std::end(vec), random::engine());
        cppsort::ska_sort(vec, &std::pair<int, float>::second);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec),
                                  std::less<>{}, &std::pair<int, float>::first) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<std::pair<int, double>> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(i, double(i));
        }
        std::shuffle(std::begin(vec), std::end(vec), random::engine());
        cppsort::ska_sort(vec, &std::pair<int, double>::second);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec),
                                  std::less<>{}, &std::pair<int, double>::first) );
    }

    SECTION( "sort with std::string iterators" )
    {
        using wrapper = generic_wrapper<std::string>;

        std::vector<wrapper> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.emplace_back(std::to_string(i));
        }
        std::shuffle(std::begin(vec), std::end(vec), random::engine());
        cppsort::ska_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec),
                                  std::less<>{}, &wrapper::value) );
    }
}
