/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <list>
#include <vector>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <catch2/catch_test_macros.hpp>

namespace
{
    // Type of sorter used for checks
    enum class sorter_type
    {
        forward,
        bidirectional,
        random_access,
        random_access2
    };

    struct forward_sorter_impl
    {
        template<typename ForwardIterator>
        auto operator()(ForwardIterator, ForwardIterator) const
            -> sorter_type
        {
            return sorter_type::forward;
        }

        using iterator_category = std::forward_iterator_tag;
    };

    struct forward_sorter:
        cppsort::sorter_facade<forward_sorter_impl>
    {};

    struct bidirectional_sorter_impl
    {
        template<typename BidirectionalIterator>
        auto operator()(BidirectionalIterator, BidirectionalIterator) const
            -> sorter_type
        {
            return sorter_type::bidirectional;
        }

        using iterator_category = std::bidirectional_iterator_tag;
    };

    struct bidirectional_sorter:
        cppsort::sorter_facade<bidirectional_sorter_impl>
    {};

    struct random_access_sorter_impl
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::random_access;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct random_access_sorter:
        cppsort::sorter_facade<random_access_sorter_impl>
    {};

    struct random_access_sorter_impl2
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::random_access2;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct random_access_sorter2:
        cppsort::sorter_facade<random_access_sorter_impl2>
    {};
}

TEST_CASE( "nested hybrid_adapter types", "[hybrid_adapter]" )
{
    // Make sure that the nesting of hybrid_adapter
    // does not hide more suitable sorters because
    // of the iterator category merging

    // Collections to sort
    std::vector<int> vec(3);
    std::list<int> list(3);

    SECTION( "unwrapping from the front" )
    {
        cppsort::hybrid_adapter<
            cppsort::hybrid_adapter<
                forward_sorter,
                random_access_sorter
            >,
            bidirectional_sorter
        > sorter;

        sorter_type res1 = sorter(vec);
        CHECK( res1 == sorter_type::random_access );

        sorter_type res2 = sorter(list);
        CHECK( res2 == sorter_type::bidirectional );
    }

    SECTION( "unwrapping from the back" )
    {
        cppsort::hybrid_adapter<
            bidirectional_sorter,
            cppsort::hybrid_adapter<
                forward_sorter,
                random_access_sorter
            >
        > sorter;

        sorter_type res = sorter(vec);
        CHECK( res == sorter_type::random_access );

        sorter_type res2 = sorter(list);
        CHECK( res2 == sorter_type::bidirectional );
    }
}

TEST_CASE( "check that nested hybrid_adapter preserves the sorters order", "[hybrid_adapter]" )
{
    cppsort::hybrid_adapter<
        random_access_sorter,
        random_access_sorter2
    > sorter;

    std::vector<int> vec(3);
    sorter_type res = sorter(vec);
    CHECK( res == sorter_type::random_access );
}
