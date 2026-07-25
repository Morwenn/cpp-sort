/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/reve.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "measure of disorder: reve", "[probe][reve]" )
{
    using cppsort::probe::reve;

    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( reve(li) == 3 );
        CHECK( reve(li.begin(), li.end()) == 3 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( reve(tricky, &internal_compare<int>::compare_to) == 3 );
    }

    SECTION( "lower bound" )
    {
        const std::forward_list<int> li1 = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( reve(li1) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to:
        // size - 2

        const std::forward_list<int> li = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
        auto max_n = reve.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 9 );
        CHECK( reve(li) == max_n );
        CHECK( reve(li.begin(), li.end()) == max_n );
    }

    SECTION( "equal neighbours in the sequence" )
    {
        const std::forward_list<int> li = { 0, 0, 0, 1, 2, 3, 4, 6, 5, 3 };
        CHECK( reve(li) == 1 );

        const std::forward_list<int> li1 = { 6, 5, 4, 3, 2, 2, 2, 2 };
        CHECK( reve(li1) == 0 );

        const std::forward_list<int> li2 = { 1, 1, 2, 8, 3, 3, 2, 1, 1, 5, 6 };
        CHECK( reve(li2) == 2 );
    }

    rc::prop("Reve(Reversed(X)) = Reve(X)", [](std::vector<int> sequence) {
        auto reve_x = reve(sequence);
        std::reverse(sequence.begin(), sequence.end());
        return reve(sequence) == reve_x;
    });

    rc::prop("Reve(Unique(X)) = Reve(X)", [](std::vector<int> sequence) {
        auto reve_x = reve(sequence);
        sequence.erase(
            std::unique(sequence.begin(), sequence.end()),
            sequence.end()
        );
        return reve(sequence) == reve_x;
    });
}
