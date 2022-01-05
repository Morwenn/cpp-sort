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
#include <cpp-sort/sorters/default_sorter.h>
#include <cpp-sort/sort.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

TEST_CASE( "default sorter tests with projections",
           "[default_sorter][projection]" )
{
    // Wrapper to hide the integer
    using wrapper = generic_wrapper<int>;

    // Collection to sort
    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    SECTION( "sort with random-access iterable" )
    {
        cppsort::sort(vec, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterable and compare" )
    {
        cppsort::sort(vec, std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators" )
    {
        cppsort::sort(std::begin(vec), std::end(vec), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with random-access iterators and compare" )
    {
        cppsort::sort(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators" )
    {
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with bidirectional iterators and compare" )
    {
        std::list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators" )
    {
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::less<>{}, &wrapper::value) );
    }

    SECTION( "sort with forward iterators and compare" )
    {
        std::forward_list<wrapper> li(std::begin(vec), std::end(vec));
        cppsort::sort(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(li), std::end(li), std::greater<>{}, &wrapper::value) );
    }
}
