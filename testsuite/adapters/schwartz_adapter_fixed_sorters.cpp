/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <array>
#include <iterator>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/fixed_sorters.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/random.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<double>;

TEST_CASE( "Schwartzian transform adapter with fixed-size sorters",
           "[schwartz_adapter]" )
{
    auto&& low_comparisons_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::low_comparisons_sorter
        >
    >{};
    auto&& low_moves_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::low_moves_sorter
        >
    >{};
    auto&& merge_exchange_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::merge_exchange_network_sorter
        >
    >{};
    auto&& odd_even_merge_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::odd_even_merge_network_sorter
        >
    >{};
    auto&& sorting_network_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::sorting_network_sorter
        >
    >{};

    SECTION( "size 0" )
    {
        std::array<wrapper, 0> collection;

        low_comparisons_sort(collection, &wrapper::value);
        low_moves_sort(collection, &wrapper::value);
        merge_exchange_sort(collection, &wrapper::value);
        odd_even_merge_sort(collection, &wrapper::value);
        sorting_network_sort(collection, &wrapper::value);
    }

    SECTION( "size 1" )
    {
        std::array<wrapper, 1> collection;

        low_comparisons_sort(collection, &wrapper::value);
        low_moves_sort(collection, &wrapper::value);
        merge_exchange_sort(collection, &wrapper::value);
        odd_even_merge_sort(collection, &wrapper::value);
        sorting_network_sort(collection, &wrapper::value);
    }

    SECTION( "size 2" )
    {
        std::array<wrapper, 2> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        odd_even_merge_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 3" )
    {
        std::array<wrapper, 3> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 4" )
    {
        std::array<wrapper, 4> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        odd_even_merge_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 5" )
    {
        std::array<wrapper, 5> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 6" )
    {
        std::array<wrapper, 6> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 31" )
    {
        std::array<wrapper, 31> collection;
        helpers::iota(collection.begin(), collection.end(), -10.0, &wrapper::value);

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 32" )
    {
        std::array<wrapper, 32> collection;
        helpers::iota(collection.begin(), collection.end(), -10.0, &wrapper::value);

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        merge_exchange_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        odd_even_merge_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(collection.begin(), collection.end(), hasard::engine());
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "stability of Schwartzian transform adapter with fixed-size sorters",
           "[schwartz_adapter][is_stable]" )
{
    using namespace cppsort;
    using sorter = schwartz_adapter<
        small_array_adapter<
            low_moves_sorter
        >
    >;

    SECTION( "is_always_stable" )
    {
        CHECK( not is_always_stable<small_array_adapter<low_moves_sorter>>::value );
        CHECK( not is_always_stable<sorter>::value );
    }
}
