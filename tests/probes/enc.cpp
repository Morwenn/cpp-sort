/*
 * Copyright (c) 2016-2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/enc.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "measure of disorder: enc", "[probe][enc]" )
{
    using cppsort::probe::enc;

    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 4, 6, 5, 2, 9, 1, 3, 8, 0, 7 };
        CHECK( enc(li) == 3 );
        CHECK( enc(li.begin(), li.end()) == 3 );

        std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( enc(tricky, &internal_compare<int>::compare_to) == 3 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to half the size
        // of the input sequence minus one

        {
            // Even number of elements
            const std::forward_list<int> li = { 11, 10, 0, 9, 1, 8, 2, 7, 3, 6, 4, 5 };
            auto max_n = enc.max_for_size(cppsort::utility::size(li));
            CHECK( max_n == 6 );
            CHECK( enc(li) == max_n );
            CHECK( enc(li.begin(), li.end()) == max_n );
        }

        {
            // Odd number of elements
            const std::forward_list<int> li = { 11, 10, 0, 9, 1, 8, 2, 7, 3, 6, 4 };
            auto max_n = enc.max_for_size(cppsort::utility::size(li));
            CHECK( max_n == 5 );
            CHECK( enc(li) == max_n );
            CHECK( enc(li.begin(), li.end()) == max_n );
        }
    }

    rc::prop("Enc(Unique(X)) = Enc(X)", [](std::vector<int> sequence) {
        auto enc_x = enc(sequence);
        sequence.erase(
            std::unique(sequence.begin(), sequence.end()),
            sequence.end()
        );
        return enc(sequence) == enc_x;
    });
}
