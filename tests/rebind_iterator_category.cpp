/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace
{
    enum struct sorter_type
    {
        foo,
        bar
    };

    struct foo_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::foo;
        }

        using iterator_category = std::forward_iterator_tag;
    };

    struct bar_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::bar;
        }

        using iterator_category = std::forward_iterator_tag;
    };

    struct foo_sorter:
        cppsort::sorter_facade<foo_sorter_impl>
    {};

    struct bar_sorter:
        cppsort::sorter_facade<bar_sorter_impl>
    {};
}

TEST_CASE( "iterator category rebinder",
           "[rebind_iterator_category][hybrid_adapter]" )
{
    SECTION( "simple case" )
    {
        using sorter1 = cppsort::rebind_iterator_category<
            foo_sorter,
            std::bidirectional_iterator_tag
        >;

        using sorter2 = cppsort::rebind_iterator_category<
            bar_sorter,
            std::random_access_iterator_tag
        >;

        STATIC_CHECK( std::is_same<cppsort::iterator_category<sorter1>,
                                   std::bidirectional_iterator_tag>::value );
        STATIC_CHECK( std::is_same<cppsort::iterator_category<sorter2>,
                                   std::random_access_iterator_tag>::value );
    }

    SECTION( "with hybrid_adapter" )
    {
        cppsort::hybrid_adapter<
            foo_sorter,
            cppsort::rebind_iterator_category<
                bar_sorter,
                std::bidirectional_iterator_tag
            >
        > sorter;

        std::forward_list<float> collection1(3);
        sorter_type res1 = sorter(collection1);
        CHECK( res1 == sorter_type::foo );

        std::list<float> collection2(3);
        sorter_type res2 = sorter(collection2);
        CHECK( res2 == sorter_type::bar );

        std::vector<float> collection3(3);
        sorter_type res3 = sorter(collection3);
        CHECK( res3 == sorter_type::bar );
    }
}
