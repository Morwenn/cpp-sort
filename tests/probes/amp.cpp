/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/amp.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "measure of disorder: amp", "[probe][amp]" )
{
    using cppsort::probe::amp;

    SECTION( "simple test" )
    {
        std::forward_list<int> li = { 4, 6, 5, 2, 9, 1, 3, 8, 0, 7 };
        CHECK( amp(li) == 7 );
        CHECK( amp(li.begin(), li.end()) == 7 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( amp(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to a sequence that
        // oscillates at every step

        std::forward_list<int> li = { 0, 2, 1, 4, 3, 6, 5, 8, 7, 10, 9 };
        auto max_n = amp.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 9 );
        CHECK( amp(li) == max_n );
        CHECK( amp(li.begin(), li.end()) == max_n );
    }

    // https://morwenn.github.io/presortedness/2025/10/18/TSB005-symmetry-of-amp.html
    rc::prop("Amp(Reversed(X)) = Amp(X)", [](std::vector<int> sequence) {
        auto amp_x = amp(sequence);
        std::reverse(sequence.begin(), sequence.end());
        return amp(sequence) == amp_x;
    });
}
