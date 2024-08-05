/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>

namespace
{
    ////////////////////////////////////////////////////////////
    // Regular mutable sorter

    struct mutable_sorter_impl
    {
        int dummy1=0, dummy2=0;

        mutable_sorter_impl() = default;
        constexpr mutable_sorter_impl(int a, int b):
            dummy1(a), dummy2(b)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = std::enable_if_t<
                cppsort::is_projection_iterator_v<Projection, Iterator, Compare>
            >
        >
        auto operator()(Iterator first, Iterator last,
                        Compare compare={}, Projection projection={})
            -> void
        {
            dummy1 = 3;
            cppsort::insertion_sort(std::move(first), std::move(last),
                                    std::move(compare), std::move(projection));
            dummy2 = 11;
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    struct mutable_sorter:
        cppsort::sorter_facade<mutable_sorter_impl>
    {
        mutable_sorter() = default;
        mutable_sorter(int a, int b):
            cppsort::sorter_facade<mutable_sorter_impl>(a, b)
        {}
    };
}


TEST_CASE( "test most adapters with a mutable sorter",
           "[adapters][sorter_facade]" )
{
    std::vector<int> collection;
    collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    // Sorter that will be adapted by everything
    auto original_sorter = mutable_sorter(5, 7);

    SECTION( "container_aware_adapter" )
    {
        cppsort::container_aware_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "hybrid_adapter" )
    {
        cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            mutable_sorter
        > adapted_sorter(cppsort::merge_sort, original_sorter);
        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.template get<1>().dummy1 == 3 );
        CHECK( adapted_sorter.template get<1>().dummy2 == 11 );
    }

    SECTION( "indirect_adapter" )
    {
        cppsort::indirect_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "out_of_place_adapter" )
    {
        cppsort::out_of_place_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "schwartz_adapter" )
    {
        cppsort::schwartz_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "self_sort_adapter" )
    {
        cppsort::self_sort_adapter adapted_sorter(original_sorter);

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        adapted_sorter(li);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
        // List sorted itself without using the original sorter
        CHECK( adapted_sorter.get().dummy1 == 5 );
        CHECK( adapted_sorter.get().dummy2 == 7 );

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        // Sorting a vector used the mutable sorter
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "stable_adapter" )
    {
        cppsort::stable_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "verge_adapter" )
    {
        cppsort::verge_adapter adapted_sorter(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }
}