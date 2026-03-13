/*
 * Copyright (c) 2018-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/mono.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "measure of disorder: mono", "[probe][mono]" )
{
    using cppsort::probe::mono;

    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
        CHECK( mono(li) == 2 );
        CHECK( mono(li.begin(), li.end()) == 2 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( mono(tricky, &internal_compare<int>::compare_to) == 2 );
    }

    SECTION( "lower bound" )
    {
        const std::forward_list<int> li1 = { 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( mono(li1) == 0 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to:
        // size / 2

        const std::forward_list<int> li = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
        auto max_n = mono.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 5 );
        CHECK( mono(li) == max_n );
        CHECK( mono(li.begin(), li.end()) == max_n );
    }

    SECTION( "equal neighbours in the sequence" )
    {
        const std::forward_list<int> li = { 0, 0, 0, 1, 2, 3, 4, 6, 5, 3 };
        CHECK( mono(li) == 1 );

        const std::forward_list<int> li1 = { 6, 5, 4, 3, 2, 2, 2, 2 };
        CHECK( mono(li1) == 0 );

        const std::forward_list<int> li2 = { 1, 1, 2, 8, 3, 3, 2, 1, 1, 5, 6 };
        CHECK( mono(li2) == 2 );
    }

    rc::prop("Mono(Reversed(X)) = Mono(X)", [](std::vector<int> sequence) {
        auto mono_x = mono(sequence);
        std::reverse(sequence.begin(), sequence.end());
        return mono(sequence) == mono_x;
    });

    rc::prop("Mono(Unique(X)) = Mono(X)", [](std::vector<int> sequence) {
        auto mono_x = mono(sequence);
        sequence.erase(
            std::unique(sequence.begin(), sequence.end()),
            sequence.end()
        );
        return mono(sequence) == mono_x;
    });
}
