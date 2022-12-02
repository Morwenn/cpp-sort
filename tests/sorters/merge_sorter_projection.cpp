/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/merge_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "merge_sorter tests with projections",
           "[merge_sorter][projection]" )
{
    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    // Collection to sort
    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    SECTION( "sort with random-access range" )
    {
        cppsort::merge_sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access range and compare" )
    {
        cppsort::merge_sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::merge_sort(vec.begin(), vec.end(), &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::merge_sort(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::list<wrapper> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), &wrapper::value);
        CHECK( helpers::is_sorted(li.begin(), li.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::list<wrapper> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(li.begin(), li.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::forward_list<wrapper> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), &wrapper::value);
        CHECK( helpers::is_sorted(li.begin(), li.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::forward_list<wrapper> li(vec.begin(), vec.end());
        cppsort::merge_sort(li.begin(), li.end(), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(li.begin(), li.end(), std::greater<>{}, &wrapper::value) );
    }
}
