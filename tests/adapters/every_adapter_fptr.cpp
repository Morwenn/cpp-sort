/*
 * Copyright (c) 2018-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <testing-tools/distributions.h>

TEST_CASE( "function pointer test for every adapter",
           "[adapters][function_pointer]" )
{
    auto distribution = dist::shuffled{};

    // Collections to sort
    std::vector<short int> collection; collection.reserve(65);
    distribution(std::back_inserter(collection), 65, 0);
    std::list<short int> li;
    distribution(std::back_inserter(li), 65, 0);
    std::forward_list<short int> fli;
    distribution(std::front_inserter(fli), 65, 0);

    SECTION( "container_aware_adapter" )
    {
        using sorter = cppsort::container_aware_adapter<
            cppsort::selection_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};
        constexpr void(*sort_it2)(std::list<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "indirect_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "out_of_place_adapter" )
    {
        using sorter = cppsort::out_of_place_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};
        constexpr void(*sort_it2)(std::list<short int>&, std::greater<>) = sorter{};
        constexpr void(*sort_it3)(std::forward_list<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );

        sort_it3(fli, std::greater<>{});
        CHECK( std::is_sorted(fli.begin(), fli.end(), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter<small_array_adapter>" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::small_array_adapter<cppsort::low_comparisons_sorter>
        >;
        constexpr void(*sort_it)(std::array<short int, 6>&, std::greater<>) = sorter{};

        std::array<short int, 6> arr = {{ 4, 3, 2, 5, 6, 1 }};
        sort_it(arr, std::greater<>{});
        CHECK( std::is_sorted(arr.begin(), arr.end(), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        using sorter = cppsort::self_sort_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};
        constexpr void(*sort_it2)(std::list<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::self_sort_adapter<cppsort::poplar_sorter>
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};
        constexpr void(*sort_it2)(std::list<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "small_array_adapter" )
    {
        using namespace cppsort;

        using sorter2 = small_array_adapter<low_moves_sorter>;
        using sorter1 = small_array_adapter<low_comparisons_sorter>;
        using sorter3 = small_array_adapter<sorting_network_sorter>;
        constexpr void(*sort_it1)(std::array<short int, 6>&, std::greater<>) = sorter2{};
        constexpr void(*sort_it2)(std::array<short int, 6>&, std::greater<>) = sorter1{};
        constexpr void(*sort_it3)(std::array<short int, 6>&, std::greater<>) = sorter3{};

        std::array<short int, 6> arr = {{ 4, 3, 2, 5, 6, 1 }};

        auto to_sort = arr;
        sort_it1(to_sort, std::greater<>{});
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<>{}) );

        to_sort = arr;
        sort_it2(to_sort, std::greater<>{});
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<>{}) );

        to_sort = arr;
        sort_it3(to_sort, std::greater<>{});
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "verge_adapter" )
    {
        using sorter = cppsort::verge_adapter<
            cppsort::poplar_sorter
        >;
        constexpr void(*sort_it)(std::vector<short int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }
}
