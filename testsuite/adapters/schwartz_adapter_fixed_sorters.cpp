/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <array>
#include <iterator>
#include <random>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/fixed_sorters.h>
#include <testing-tools/algorithm.h>

TEST_CASE( "Schwartzian transform adapter with fixed-size sorters",
           "[schwartz_adapter]" )
{
    struct wrapper { double value; };

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
    auto&& sorting_network_sort = cppsort::schwartz_adapter<
        cppsort::small_array_adapter<
            cppsort::sorting_network_sorter
        >
    >{};

    std::mt19937 engine(Catch::rngSeed());

    SECTION( "size 0" )
    {
        std::array<wrapper, 0> collection;

        low_comparisons_sort(collection, &wrapper::value);
        low_moves_sort(collection, &wrapper::value);
        sorting_network_sort(collection, &wrapper::value);
    }

    SECTION( "size 1" )
    {
        std::array<wrapper, 1> collection;

        low_comparisons_sort(collection, &wrapper::value);
        low_moves_sort(collection, &wrapper::value);
        sorting_network_sort(collection, &wrapper::value);
    }

    SECTION( "size 2" )
    {
        std::array<wrapper, 2> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 3" )
    {
        std::array<wrapper, 3> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 4" )
    {
        std::array<wrapper, 4> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 5" )
    {
        std::array<wrapper, 5> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "size 6" )
    {
        std::array<wrapper, 6> collection;
        helpers::iota(std::begin(collection), std::end(collection), -10.0, &wrapper::value);

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_comparisons_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        low_moves_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );

        std::shuffle(std::begin(collection), std::end(collection), engine);
        sorting_network_sort(collection, &wrapper::value);
        CHECK( helpers::is_sorted(std::begin(collection), std::end(collection),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "stability of Schwartzian transform adapter with fixed-size sorters",
           "[schwartz_adapter][is_stable]" )
{
    struct wrapper { double value; };

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
