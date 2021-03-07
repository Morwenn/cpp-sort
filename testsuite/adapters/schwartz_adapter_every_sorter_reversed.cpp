/*
 * Copyright (c) 2017-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <random>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

// NOTE: this test used to use wrapper<double>, but it was later
//       switched to wrapper<float> because the former segfaults
//       with MinGW-w64 64 bits in Release mode
template<typename T=float>
using wrapper = generic_wrapper<T>;

TEMPLATE_TEST_CASE( "every sorter with Schwartzian transform adapter and reverse iterators",
                    "[schwartz_adapter][reverse_iterator]",
                    cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>,
                    cppsort::cartesian_tree_sorter,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter )
{
    std::vector<wrapper<>> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 412, -125);

    cppsort::schwartz_adapter<TestType> sorter;
    sorter(collection.rbegin(), collection.rend(), &wrapper<>::value);
    CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                              std::greater<>{}, &wrapper<>::value) );
}

TEST_CASE( "type-specific sorters with Schwartzian transform adapter and reverse iterators",
           "[schwartz_adapter][reverse_iterator]" )
{
    auto distribution = dist::shuffled{};

    std::vector<wrapper<>> collection;
    distribution(std::back_inserter(collection), 412, -125);

    std::vector<wrapper<int>> collection2;
    distribution(std::back_inserter(collection2), 412, -125);

    std::vector<wrapper<std::string>> collection3;
    for (int i = -125 ; i < 287 ; ++i) {
        collection3.emplace_back(std::to_string(i));
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection3.begin(), collection3.end(), engine);

    SECTION( "ska_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::ska_sorter> sorter;

        sorter(collection.rbegin(), collection.rend(), &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::greater<>{}, &wrapper<>::value) );

        sorter(collection2.rbegin(), collection2.rend(), &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::greater<>{}, &wrapper<int>::value) );

        sorter(collection3.rbegin(), collection3.rend(), &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::greater<>{}, &wrapper<std::string>::value) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::spread_sorter> sorter;

        sorter(collection.rbegin(), collection.rend(), &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::greater<>{}, &wrapper<>::value) );

        sorter(collection2.rbegin(), collection2.rend(), &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::greater<>{}, &wrapper<int>::value) );

        sorter(collection3.rbegin(), collection3.rend(), &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::greater<>{}, &wrapper<std::string>::value) );

        std::shuffle(collection3.begin(), collection3.end(), engine);
        sorter(collection3.rbegin(), collection3.rend(),
               std::greater<>{}, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::less<>{}, &wrapper<std::string>::value) );
    }
}
