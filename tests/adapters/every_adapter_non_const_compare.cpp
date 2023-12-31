/*
 * Copyright (c) 2020-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstddef>
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>

namespace
{
    // identity, but avoids special cases
    struct fake_identity
    {
        template<typename T>
        constexpr auto operator()(T&& value) const noexcept
            -> T&&
        {
            return std::forward<T>(value);
        }
    };
}

TEST_CASE( "test adapters extended compatibility with LWG 3031", "[adapters]" )
{
    auto distribution = dist::shuffled{};

    std::vector<int> vec; vec.reserve(65);
    distribution(std::back_inserter(vec), 65, 0);
    std::list<int> li;
    distribution(std::back_inserter(li), 65, 0);

    auto non_const_compare = [](int& lhs, int& rhs) { return lhs < rhs; };

    SECTION( "hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "indirect_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "out_of_place_adapter" )
    {
        using sorter = cppsort::out_of_place_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        sorter{}(li, non_const_compare);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "schwartz_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::detail::poplar_sorter_impl
        >;

        sorter{}(vec, non_const_compare, fake_identity{});
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "self_sort_adapter" )
    {
        using sorter = cppsort::self_sort_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        sorter{}(li, non_const_compare);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::self_sort_adapter<cppsort::poplar_sorter>
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        sorter{}(li, non_const_compare);
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "small_array_adapter" )
    {
        using namespace cppsort;

        std::array<int, 6> arr = {{ 4, 3, 2, 5, 6, 1 }};

        auto to_sort = arr;
        small_array_adapter<low_comparisons_sorter>{}(to_sort, non_const_compare);
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end()) );

        to_sort = arr;
        small_array_adapter<low_moves_sorter>{}(to_sort, non_const_compare);
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end()) );

        to_sort = arr;
        small_array_adapter<sorting_network_sorter>{}(to_sort, non_const_compare);
        CHECK( std::is_sorted(to_sort.begin(), to_sort.end()) );
    }

    SECTION( "stable_adapter" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "verge_adapter" )
    {
        using sorter = cppsort::verge_adapter<
            cppsort::poplar_sorter
        >;

        sorter{}(vec, non_const_compare);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}
