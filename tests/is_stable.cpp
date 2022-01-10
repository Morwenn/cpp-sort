/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/fixed/low_comparisons_sorter.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/quick_sorter.h>
#include <cpp-sort/utility/functional.h>

TEST_CASE( "test is_stable with raw sorters",
           "[is_stable]" )
{
    // Exhaustive test for every scenario where is_sorter is
    // supposed to trivially fall back to is_always_stable

    using cppsort::is_stable;
    using cppsort::utility::identity;

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::merge_sorter;

        STATIC_CHECK( is_stable<sorter(std::vector<int>&)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::less<>)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::less<>)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::greater<>)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::greater<>)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, identity)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       identity)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::negate<>)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::negate<>)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::less<>, identity)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::less<>, identity)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::greater<>, identity)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::greater<>, identity)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::less<>, std::negate<>)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::less<>, std::negate<>)>::value );

        STATIC_CHECK( is_stable<sorter(std::vector<int>&, std::greater<>, std::negate<>)>::value );
        STATIC_CHECK( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                       std::greater<>, std::negate<>)>::value );
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::quick_sorter;

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::less<>)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::less<>)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::greater<>)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::greater<>)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, identity)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           identity)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::negate<>)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::negate<>)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::less<>, identity)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::less<>, identity)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::greater<>, identity)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::greater<>, identity)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::less<>, std::negate<>)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::less<>, std::negate<>)>::value );

        STATIC_CHECK( not is_stable<sorter(std::vector<int>&, std::greater<>, std::negate<>)>::value );
        STATIC_CHECK( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                           std::greater<>, std::negate<>)>::value );
    }

    SECTION( "low_comparisons_sorter" )
    {
        // Many fixed-size sorters define is_always_stable through
        // sorter_traits, which makes this test interesting
        using sorter = cppsort::low_comparisons_sorter<8>;
        STATIC_CHECK( not is_stable<sorter(int(&)[8])>::value );
    }
}
