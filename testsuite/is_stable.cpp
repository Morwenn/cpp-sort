/*
 * Copyright (c) 2016-2018 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <vector>
#include <catch2/catch.hpp>
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

        CHECK(( is_stable<sorter(std::vector<int>&)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::less<>)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::less<>)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::greater<>)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::greater<>)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, identity)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 identity)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::negate<>)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::negate<>)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::less<>, identity)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::less<>, identity)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::greater<>, identity)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::greater<>, identity)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::less<>, std::negate<>)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::less<>, std::negate<>)>::value ));

        CHECK(( is_stable<sorter(std::vector<int>&, std::greater<>, std::negate<>)>::value ));
        CHECK(( is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                 std::greater<>, std::negate<>)>::value ));
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::quick_sorter;

        CHECK(( not is_stable<sorter(std::vector<int>&)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::less<>)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::less<>)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::greater<>)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::greater<>)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, identity)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     identity)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::negate<>)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::negate<>)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::less<>, identity)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::less<>, identity)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::greater<>, identity)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::greater<>, identity)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::less<>, std::negate<>)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::less<>, std::negate<>)>::value ));

        CHECK(( not is_stable<sorter(std::vector<int>&, std::greater<>, std::negate<>)>::value ));
        CHECK(( not is_stable<sorter(std::vector<int>::iterator, std::vector<int>::iterator,
                                     std::greater<>, std::negate<>)>::value ));
    }

    SECTION( "low_comparisons_sorter" )
    {
        // Many fixed-size sorters define is_always_stable through
        // sorter_traits, which makes this test interesting
        using sorter = cppsort::low_comparisons_sorter<8>;
        CHECK( not is_stable<sorter(int(&)[8])>::value );
    }
}
