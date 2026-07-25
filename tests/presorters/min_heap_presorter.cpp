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
#include <cpp-sort/presorters/min_heap_presorter.h>

TEST_CASE( "test min_heap_presorter with RapidCheck", "[presorter]" )
{
    rc::prop("with std::vector", [](std::vector<int> collection) {
        cppsort::min_heap_presort(collection);
        return std::is_heap(collection.begin(), collection.end(), std::greater{});
    });

    rc::prop("with std::deque", [](std::deque<int> collection) {
        cppsort::min_heap_presort(collection);
        return std::is_heap(collection.begin(), collection.end(), std::greater{});
    });
}
