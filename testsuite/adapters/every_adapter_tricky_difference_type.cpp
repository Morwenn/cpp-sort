/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstdint>
#include <iterator>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <testing-tools/distributions.h>
#include <testing-tools/test_vector.h>

TEMPLATE_TEST_CASE( "test adapters with an int8_t difference_type", "[adapters]",
                    cppsort::indirect_adapter<cppsort::poplar_sorter>,
                    cppsort::out_of_place_adapter<cppsort::poplar_sorter>,
                    cppsort::schwartz_adapter<cppsort::poplar_sorter>,
                    cppsort::stable_adapter<cppsort::poplar_sorter>,
                    cppsort::verge_adapter<cppsort::poplar_sorter>
                     )
{
    // Test that adapters work as expected when the iterator to sort has
    // a small difference_type, despite potential promotions to int

    test_vector<int, std::int8_t> collection(127);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 127);

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
