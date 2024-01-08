/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>

namespace
{
    enum class sorter_type
    {
        bidirectional,
        random_access,
        contiguous
    };

    template<int N>
    struct contiguous_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::contiguous;
        }

        using iterator_category = std::contiguous_iterator_tag;
    };

    struct random_access_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::random_access;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct bidirectional_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::bidirectional;
        }

        using iterator_category = std::bidirectional_iterator_tag;
    };

    template<int N>
    struct contiguous_sorter:
        cppsort::sorter_facade<contiguous_sorter_impl<N>>
    {};

    struct random_access_sorter:
        cppsort::sorter_facade<random_access_sorter_impl>
    {};

    struct bidirectional_sorter:
        cppsort::sorter_facade<bidirectional_sorter_impl>
    {};
}

TEST_CASE( "hybrid_adapter with many sorters",
           "[hybrid_adapter]" )
{
    // Check that hybrid_adapter scales even when there are
    // a great many sorters involved, especially check that
    // the indices scale well

    cppsort::hybrid_adapter<
        bidirectional_sorter,
        random_access_sorter,
        contiguous_sorter<0>,
        contiguous_sorter<1>,
        contiguous_sorter<2>,
        contiguous_sorter<3>,
        contiguous_sorter<4>,
        contiguous_sorter<5>
    > sorter;

    std::vector<int> vec;
    auto res1 = sorter(vec);
    CHECK( res1 == sorter_type::contiguous );

    std::list<int> li;
    auto res2 = sorter(li);
    CHECK( res2 == sorter_type::bidirectional );
}
