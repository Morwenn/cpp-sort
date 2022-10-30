/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <utility>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/end_sentinel.h>

TEMPLATE_TEST_CASE( "every probe with a sentinel", "[probe][sentinel]",
                     decltype(cppsort::probe::block),
                     decltype(cppsort::probe::dis),
                     decltype(cppsort::probe::enc),
                     decltype(cppsort::probe::exc),
                     decltype(cppsort::probe::ham),
                     decltype(cppsort::probe::inv),
                     decltype(cppsort::probe::max),
                     decltype(cppsort::probe::mono),
                     decltype(cppsort::probe::osc),
                     decltype(cppsort::probe::rem),
                     decltype(cppsort::probe::runs),
                     decltype(cppsort::probe::sus) )
{
    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 500, -200);

    using iterator = decltype(collection.begin());
    using sentinel = end_sentinel<iterator>;
    static_assert(cppsort::mstd::sentinel_for<sentinel, iterator>);
    static_assert(cppsort::is_projection_iterator_v<std::identity, iterator>);

    TestType probe;
    auto presortedness1 = probe(collection);
    auto end = end_sentinel(collection.end());
    auto presortedness2 = probe(collection.begin(), end);
    CHECK( presortedness1 == presortedness2 );
}
