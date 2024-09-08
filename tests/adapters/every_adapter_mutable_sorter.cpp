/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/mutable_sorter.h>

TEST_CASE( "test most adapters with a mutable sorter",
           "[adapters][sorter_facade]" )
{
    std::vector<int> collection;
    collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    // Sorter that will be adapted by everything,
    // works with forward iterators
    auto original_sorter = mutable_sorter{};

    SECTION( "container_aware_adapter" )
    {
        cppsort::container_aware_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "hybrid_adapter" )
    {
        std::list<int> li(collection.begin(), collection.end());

        cppsort::hybrid_adapter<
            cppsort::pdq_sorter,
            mutable_sorter
        > adapted_sorter(cppsort::pdq_sort, original_sorter);
        adapted_sorter(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );
        CHECK( adapted_sorter.template get<1>().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.template get<1>().after_sort == mutable_state::modified );
    }

    SECTION( "indirect_adapter" )
    {
        cppsort::indirect_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "out_of_place_adapter" )
    {
        cppsort::out_of_place_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "schwartz_adapter" )
    {
        cppsort::schwartz_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "self_sort_adapter" )
    {
        cppsort::self_sort_adapter adapted_sorter(original_sorter);

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        adapted_sorter(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );
        // List sorted itself without using the original sorter
        CHECK( adapted_sorter.get().before_sort == mutable_state::initial );
        CHECK( adapted_sorter.get().after_sort == mutable_state::initial );

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        // Sorting a vector used the mutable sorter
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "stable_adapter" )
    {
        cppsort::stable_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }

    SECTION( "stable_adapter<self_sorter_adapter>" )
    {
        cppsort::self_sort_adapter inner_adapter(original_sorter);
        cppsort::stable_adapter adapted_sorter(inner_adapter);

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        adapted_sorter(li);
        CHECK( std::is_sorted(li.begin(), li.end()) );
        // List sorted itself without using the original sorter
        CHECK( adapted_sorter.get().get().before_sort == mutable_state::initial );
        CHECK( adapted_sorter.get().get().after_sort == mutable_state::initial );

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        // Sorting a vector used the mutable sorter
        CHECK( adapted_sorter.get().get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().get().after_sort == mutable_state::modified );
    }

    SECTION( "verge_adapter" )
    {
        cppsort::verge_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
        CHECK( adapted_sorter.get().before_sort == mutable_state::modified );
        CHECK( adapted_sorter.get().after_sort == mutable_state::modified );
    }
}
