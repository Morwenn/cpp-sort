/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <type_traits>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

namespace
{
    struct dummy_t {};

    struct sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator, Iterator, Compare={}, Projection={}) const
            -> dummy_t
        {
            return {};
        }
    };

    struct sorter:
        cppsort::sorter_facade<sorter_impl>
    {};
}

TEST_CASE( "check conversions from sorter_facade to fptr",
           "[sorter_facade][function_pointer]" )
{
    // Collection to "sort"
    std::vector<int> vec;
    sorter sort_it;

    // The tests in this section are just compilation tests

    SECTION( "conversion to function pointer" )
    {
        constexpr dummy_t(*sort1)(std::vector<int>&) = sort_it;
        constexpr dummy_t(*sort2)(std::vector<int>&, std::greater<>) = sort_it;
        constexpr dummy_t(*sort3)(std::vector<int>&, std::negate<>) = sort_it;
        constexpr dummy_t(*sort4)(std::vector<int>&, std::greater<>, std::negate<>) = sort_it;
        constexpr dummy_t(*sort5)(std::vector<int>::iterator, std::vector<int>::iterator) = sort_it;
        constexpr dummy_t(*sort6)(std::vector<int>::iterator, std::vector<int>::iterator,
                                  std::greater<>) = sort_it;
        constexpr dummy_t(*sort7)(std::vector<int>::iterator, std::vector<int>::iterator,
                                  std::negate<>) = sort_it;
        constexpr dummy_t(*sort8)(std::vector<int>::iterator, std::vector<int>::iterator,
                                  std::greater<>, std::negate<>) = sort_it;

        (void)sort1;
        (void)sort2;
        (void)sort3;
        (void)sort4;
        (void)sort5;
        (void)sort6;
        (void)sort7;
        (void)sort8;

        CHECK( true );
    }

    SECTION( "conversion to function pointer with void return type" )
    {
        constexpr void(*sort1)(std::vector<int>&) = sort_it;
        constexpr void(*sort2)(std::vector<int>&, std::greater<>) = sort_it;
        constexpr void(*sort3)(std::vector<int>&, std::negate<>) = sort_it;
        constexpr void(*sort4)(std::vector<int>&, std::greater<>, std::negate<>) = sort_it;
        constexpr void(*sort5)(std::vector<int>::iterator, std::vector<int>::iterator) = sort_it;
        constexpr void(*sort6)(std::vector<int>::iterator, std::vector<int>::iterator,
                               std::greater<>) = sort_it;
        constexpr void(*sort7)(std::vector<int>::iterator, std::vector<int>::iterator,
                               std::negate<>) = sort_it;
        constexpr void(*sort8)(std::vector<int>::iterator, std::vector<int>::iterator,
                               std::greater<>, std::negate<>) = sort_it;

        (void)sort1;
        (void)sort2;
        (void)sort3;
        (void)sort4;
        (void)sort5;
        (void)sort6;
        (void)sort7;
        (void)sort8;

        CHECK( true );
    }
}
