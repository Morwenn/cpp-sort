/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <functional>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/comparators/flip.h>
#include <cpp-sort/comparators/not_fn.h>
#include <cpp-sort/presorters/reverse_descending_runs_presorter.h>
#include <cpp-sort/probes/mono.h>
#include <cpp-sort/probes/runs.h>

TEST_CASE( "basic reverse_descending_runs_presorter tests", "[presorter]" )
{
    std::vector<int> vec = { 5, 4, 3, 2, 3, 6, 6, 7, 9, 8, 8, 7, 6, 5, 5, 2, 4, 9 };

    SECTION( "with std::less" )
    {
        cppsort::reverse_descending_runs_presort(vec);
        std::vector<int> expected = { 2, 3, 4, 5, 3, 6, 6, 7, 9, 8, 8, 5, 6, 7, 2, 5, 4, 9 };
        CHECK( vec == expected );
    }

    SECTION( "with std::less_equal" )
    {
        cppsort::reverse_descending_runs_presort(vec, std::less_equal{});
        std::vector<int> expected = { 2, 3, 4, 5, 3, 6, 6, 7, 9, 2, 5, 5, 6, 7, 8, 8, 4, 9 };
        CHECK( vec == expected );
    }

    SECTION( "with not_fn(flip(std::less))" )
    {
        cppsort::reverse_descending_runs_presort(
            vec, cppsort::not_fn(cppsort::flip(std::less{}))
        );
        std::vector<int> expected = { 2, 3, 4, 5, 3, 6, 6, 7, 9, 2, 5, 5, 6, 7, 8, 8, 4, 9 };
        CHECK( vec == expected );
    }
}

TEST_CASE( "test reverse_descending_runs_presorter with RapidCheck", "[presorter]" )
{
    rc::prop("with std::deque", [](std::deque<int> collection) {
        auto mono = cppsort::probe::mono(collection);
        cppsort::reverse_descending_runs_presort(collection, std::less_equal{});
        auto runs = cppsort::probe::runs(collection);
        return runs <= mono;
    });

    rc::prop("with std::list", [](std::list<int> collection) {
        auto mono = cppsort::probe::mono(collection);
        cppsort::reverse_descending_runs_presort(collection, std::less_equal{});
        auto runs = cppsort::probe::runs(collection);
        return runs <= mono;
    });
}
