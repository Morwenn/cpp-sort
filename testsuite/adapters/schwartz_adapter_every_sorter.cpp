/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <list>
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

TEMPLATE_TEST_CASE( "every random-access sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
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
    sorter(collection, &wrapper<>::value);
    CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                              std::less<>{}, &wrapper<>::value) );
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::verge_sorter )
{
    std::list<wrapper<>> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 412, -125);

    cppsort::schwartz_adapter<TestType> sorter;
    sorter(collection, &wrapper<>::value);
    CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                              std::less<>{}, &wrapper<>::value) );
}

TEMPLATE_TEST_CASE( "every forward sorter with Schwartzian transform adapter", "[schwartz_adapter]",
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    std::forward_list<wrapper<>> collection;
    auto distribution = dist::shuffled{};
    distribution(std::front_inserter(collection), 412, -125);

    cppsort::schwartz_adapter<TestType> sorter;
    sorter(collection, &wrapper<>::value);
    CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                              std::less<>{}, &wrapper<>::value) );
}

TEST_CASE( "type-specific sorters with Schwartzian transform adapter", "[schwartz_adapter]" )
{
    auto distribution = dist::shuffled{};

    std::vector<wrapper<>> collection(412);
    distribution(std::back_inserter(collection), 412, -125);

    std::vector<wrapper<int>> collection2(412);
    distribution(std::back_inserter(collection2), 412, -125);

    std::vector<wrapper<std::string>> collection3;
    for (int i = -125 ; i < 287 ; ++i) {
        collection3.push_back({std::to_string(i)});
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(collection3.begin(), collection3.end(), engine);

    SECTION( "ska_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::ska_sorter> sorter;

        sorter(collection, &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper<>::value) );

        sorter(collection2, &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::less<>{}, &wrapper<int>::value) );

        sorter(collection3, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::less<>{}, &wrapper<std::string>::value) );
    }

    SECTION( "spread_sorter" )
    {
        cppsort::schwartz_adapter<cppsort::spread_sorter> sorter;

        sorter(collection, &wrapper<>::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper<>::value) );

        sorter(collection2, &wrapper<int>::value);
        CHECK( helpers::is_sorted(collection2.begin(), collection2.end(),
                                  std::less<>{}, &wrapper<int>::value) );

        sorter(collection3, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::less<>{}, &wrapper<std::string>::value) );

        std::shuffle(collection3.begin(), collection3.end(), engine);
        sorter(collection3, std::greater<>{}, &wrapper<std::string>::value);
        CHECK( helpers::is_sorted(collection3.begin(), collection3.end(),
                                  std::greater<>{}, &wrapper<std::string>::value) );
    }
}
