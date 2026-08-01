/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <type_traits>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes.h>
#include "testing-tools/random.h"

//
// Test extra commonenough properties of measures of disorder:
// - Whether neighbors that compare equivalent affect the sequence
// - Horizontal symmetry
//

TEMPLATE_TEST_CASE( "test measures that ignore equal neighbors", "[probe]",
                    decltype(cppsort::probe::amp),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::reve),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    // The literature about measures of disorder generally only considers
    // sequences of distinct elements. This test checks that some of them
    // return the same result with neighboring elements that compare
    // equivalent, and when reducing these groups to one element.

    rc::prop("M(Unique(X)) = M(X)", [](std::vector<int> sequence) {
        std::decay_t<TestType> measure;

        auto measure_x = measure(sequence);
        sequence.erase(
            std::unique(sequence.begin(), sequence.end()),
            sequence.end()
        );
        return measure(sequence) == measure_x;
    });
}

TEMPLATE_TEST_CASE( "test measures that find the same disorder in a reversed sequence", "[probe]",
                    decltype(cppsort::probe::amp),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::reve) )
{
    // Several measures of presortedness find as much disorder in a sequence,
    // than in the same sequence flipped horizontally (reversed)
    // Those tend to violate Mannila's 4th axiom:
    // https://morwenn.github.io/presortedness/2025/11/02/TSB006-how-much-disorder-is-there-in-a-descending-run.html

    rc::prop("M(Reversed(X)) = M(X)", [](std::vector<int> sequence) {
        std::decay_t<TestType> measure;

        auto measure_x = measure(sequence);
        std::reverse(sequence.begin(), sequence.end());
        return measure(sequence) == measure_x;
    });
}
