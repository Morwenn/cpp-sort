/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <concepts>
#include <iterator>
#include <string>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <testing-tools/span.h>

namespace
{
    // Type of sorter used for checks
    enum class sorter_type
    {
        integer,
        floating_point,
        generic
    };

    struct integer_sorter_impl
    {
        template<typename RandomAccessIterator>
            requires std::integral<std::iter_value_t<RandomAccessIterator>>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::integer;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct float_sorter_impl
    {
        template<typename RandomAccessIterator>
            requires std::floating_point<std::iter_value_t<RandomAccessIterator>>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::floating_point;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct generic_sorter_impl
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::generic;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct integer_sorter:
        cppsort::sorter_facade<integer_sorter_impl>
    {};

    struct float_sorter:
        cppsort::sorter_facade<float_sorter_impl>
    {};

    struct generic_sorter:
        cppsort::sorter_facade<generic_sorter_impl>
    {};
}

TEST_CASE( "sfinae forwarding in hybrid_adapter",
           "[hybrid_adapter][sfinae]" )
{
    // Check that hybrid_adapter takes into account
    // the SFINAE in the aggregated sorters

    // Vectors to "sort"
    std::vector<int> vec1(3);
    std::vector<float> vec2(3);
    std::vector<std::string> vec3(3);

    cppsort::hybrid_adapter<
        float_sorter,
        integer_sorter,
        // Should act as a fallback
        generic_sorter
    > sorter;

    SECTION( "with iterators" )
    {
        sorter_type res1 = sorter(vec1.begin(), vec1.end());
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2.begin(), vec2.end());
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3.begin(), vec3.end());
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with ranges" )
    {
        sorter_type res1 = sorter(vec1);
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2);
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3);
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with span" )
    {
        sorter_type res1 = sorter(make_span(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(make_span(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(make_span(vec3));
        CHECK( res3 == sorter_type::generic );
    }
}

TEST_CASE( "sfinae forwarding in nested hybrid_adapter",
           "[hybrid_adapter][sfinae]" )
{
    // Check that hybrid_adapter takes into account
    // the SFINAE in the aggregated sorters

    // Vectors to "sort"
    std::vector<int> vec1(3);
    std::vector<float> vec2(3);
    std::vector<std::string> vec3(3);

    cppsort::hybrid_adapter<
        cppsort::hybrid_adapter<
            float_sorter,
            integer_sorter
        >,
        // Should act as a fallback
        generic_sorter
    > sorter;

    SECTION( "with iterators" )
    {
        sorter_type res1 = sorter(vec1.begin(), vec1.end());
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2.begin(), vec2.end());
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3.begin(), vec3.end());
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with ranges" )
    {
        sorter_type res1 = sorter(vec1);
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2);
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3);
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with span" )
    {
        sorter_type res1 = sorter(make_span(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(make_span(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(make_span(vec3));
        CHECK( res3 == sorter_type::generic );
    }
}
