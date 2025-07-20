/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <array>
#include <forward_list>
#include <iterator>
#include <list>
#include <utility>
#include <vector>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <catch2/catch_test_macros.hpp>

TEST_CASE( "hybrid_adapter stability checks",
           "[hybrid_adapter][is_stable]" )
{
    SECTION( "simple sorters" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::selection_sorter,
            cppsort::rebind_iterator_category<
                cppsort::merge_sorter,
                std::bidirectional_iterator_tag
            >,
            cppsort::pdq_sorter
        >;

        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::vector<int>&)> );
        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::vector<int>::iterator,
                                                      std::vector<int>::iterator)> );

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>::iterator,
                                                  std::list<int>::iterator)> );

        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::forward_list<int>&)> );
        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::forward_list<int>::iterator,
                                                      std::forward_list<int>::iterator)> );
    }

    SECTION( "nested hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::selection_sorter,
            cppsort::hybrid_adapter<
                cppsort::rebind_iterator_category<
                    cppsort::merge_sorter,
                    std::bidirectional_iterator_tag
                >
            >,
            cppsort::pdq_sorter
        >;

        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::vector<int>&)> );
        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::vector<int>::iterator,
                                                      std::vector<int>::iterator)> );

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>::iterator,
                                                  std::list<int>::iterator)> );

        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::forward_list<int>&)> );
        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::forward_list<int>::iterator,
                                                      std::forward_list<int>::iterator)> );
    }

    SECTION( "with small_array_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::small_array_adapter<
                cppsort::sorting_network_sorter,
                std::make_index_sequence<14u>
            >,
            cppsort::merge_sorter
        >;

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::vector<int>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::vector<int>::iterator,
                                                  std::vector<int>::iterator)> );

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::list<int>::iterator,
                                                  std::list<int>::iterator)> );

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::forward_list<int>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::forward_list<int>::iterator,
                                                  std::forward_list<int>::iterator)> );

        STATIC_CHECK( not cppsort::is_stable_v<sorter(std::array<int, 5>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::array<int, 5>::iterator,
                                                  std::array<int, 5>::iterator)> );

        STATIC_CHECK( cppsort::is_stable_v<sorter(std::array<int, 16>&)> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(std::array<int, 16>::iterator,
                                                  std::array<int, 16>::iterator)> );

        STATIC_CHECK( not cppsort::is_stable_v<sorter(int(&)[8])> );
        STATIC_CHECK( cppsort::is_stable_v<sorter(int(&)[20])> );
    }
}
