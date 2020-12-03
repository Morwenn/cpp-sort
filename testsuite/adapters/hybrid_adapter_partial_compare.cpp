/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>

namespace
{
    enum struct sorter_type
    {
        ascending,
        descending,
        generic
    };

    struct partial_comparison_sorter_impl
    {
        // Sort in ascending order
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::ascending;
        }

        // Sort in descending order
        template<typename Iterator>
        auto operator()(Iterator, Iterator, std::greater<>) const
            -> sorter_type
        {
            return sorter_type::descending;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct generic_sorter_impl
    {
        template<typename Iterator, typename Compare>
        auto operator()(Iterator, Iterator, Compare) const
            -> sorter_type
        {
            return sorter_type::generic;
        }

        using iterator_category = std::forward_iterator_tag;
    };

    struct partial_comparison_sorter:
        cppsort::sorter_facade<partial_comparison_sorter_impl>
    {};

    struct generic_sorter:
        cppsort::sorter_facade<generic_sorter_impl>
    {};
}

TEST_CASE( "hybrid_adapter over partial comparison sorter",
           "[hybrid_adapter][compare]" )
{
    // Check that hybrid_adapter works as expected even
    // with partial comparison sorters

    cppsort::hybrid_adapter<
        partial_comparison_sorter,
        generic_sorter
    > sorter;

    // Vector to "sort"
    std::vector<int> vec(3);

    SECTION( "without a comparison function" )
    {
        sorter_type res1 = sorter(vec);
        CHECK( res1 == sorter_type::ascending );

        sorter_type res2 = sorter(std::begin(vec), std::end(vec));
        CHECK( res2 == sorter_type::ascending );
    }

    SECTION( "with std::less<>" )
    {
        sorter_type res1 = sorter(vec, std::less<>{});
        CHECK( res1 == sorter_type::ascending );

        sorter_type res2 = sorter(std::begin(vec), std::end(vec), std::less<>{});
        CHECK( res2 == sorter_type::ascending );
    }

    SECTION( "with std::greater<>" )
    {
        sorter_type res1 = sorter(vec, std::greater<>{});
        CHECK( res1 == sorter_type::descending );

        sorter_type res2 = sorter(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( res2 == sorter_type::descending );
    }

    SECTION( "with another function object" )
    {
        sorter_type res1 = sorter(vec, std::less_equal<>{});
        CHECK( res1 == sorter_type::generic );

        sorter_type res2 = sorter(std::begin(vec), std::end(vec), std::less_equal<>{});
        CHECK( res2 == sorter_type::generic );
    }
}
