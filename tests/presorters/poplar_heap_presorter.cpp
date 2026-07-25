/*
 * Copyright (c) 2026 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <functional>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/presorters/poplar_heap_presorter.h>
#include <testing-tools/algorithm.h>

TEST_CASE( "test poplar_heap_presorter with RapidCheck", "[presorter]" )
{
    rc::prop("with std::vector", [](std::vector<int> collection) {
        cppsort::poplar_heap_presort(collection);
        return helpers::is_poplar_heap(collection.begin(), collection.end());
    });

    rc::prop("with std::deque", [](std::deque<int> collection) {
        cppsort::poplar_heap_presort(collection);
        return helpers::is_poplar_heap(collection.begin(), collection.end());
    });
}
