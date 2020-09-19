/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <list>
#include <type_traits>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/container_aware_adapter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <testing-tools/algorithm.h>

namespace foobar
{
    template<typename T>
    struct cool_list:
        std::list<T>
    {
        using std::list<T>::list;
    };

    template<
        typename T,
        typename Compare,
        typename = std::enable_if_t<
            not cppsort::is_projection<Compare, T>::value
        >
    >
    auto sort(cppsort::merge_sorter, cool_list<T>&, Compare)
        -> bool
    {
        return true;
    }
}

TEST_CASE( "basic tests with container_aware_adapter",
           "[container_aware_adapter]" )
{
    using sorter_t = cppsort::container_aware_adapter<
        cppsort::merge_sorter
    >;
    sorter_t sorter;

    // Cool list to "sort"
    foobar::cool_list<int> collection;

    SECTION( "with comparison" )
    {
        CHECK( sorter(collection, std::greater<>{}) );
        CHECK( not cppsort::is_stable<sorter_t(foobar::cool_list<int>&, std::greater<>)>::value );
    }

    SECTION( "with projection" )
    {
        CHECK( sorter(collection, std::negate<>{}) );
        CHECK( not cppsort::is_stable<sorter_t(foobar::cool_list<int>&, std::negate<>)>::value );
    }

    SECTION( "with automagic comparison-projection" )
    {
        CHECK( sorter(collection, std::greater<>{}, std::negate<>{}) );
        CHECK( not cppsort::is_stable<sorter_t(foobar::cool_list<int>&, std::greater<>, std::negate<>)>::value );
    }

    SECTION( "more about stability" )
    {
        CHECK( cppsort::is_stable<sorter_t(std::list<int>&)>::value );
        CHECK( cppsort::is_stable<sorter_t(std::list<int>::iterator, std::list<int>::iterator)>::value );
        CHECK( cppsort::is_stable<sorter_t(foobar::cool_list<int>::iterator,
                                           foobar::cool_list<int>::iterator)>::value );
    }
}
