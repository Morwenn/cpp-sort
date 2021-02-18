/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/memory_exhaustion.h>

TEMPLATE_TEST_CASE( "test every probe with all_equal distribution", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    // Ensure that all measures of presortedness return 0 when
    // given a collection where all elements are equal

    std::vector<int> collection(50, 5);
    cppsort::detail::remove_cvref_t<TestType> mop;
    auto presortedness = mop(collection);
    CHECK( presortedness == 0 );
}
