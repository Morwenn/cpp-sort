/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <testing-tools/end_sentinel.h>
#include <testing-tools/wrapper.h>

namespace
{
    namespace mstd = cppsort::mstd;

    struct comparison_sorter_impl
    {
        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare = std::less<>
        >
        auto operator()(Iterator, Sentinel, Compare={}) const
            -> bool
        {
            return true;
        }
    };

    struct projection_sorter_impl
    {
        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Projection = std::identity
        >
        auto operator()(Iterator, Sentinel, Projection={}) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<
            mstd::forward_iterator Iterator,
            mstd::sentinel_for<Iterator> Sentinel,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_iterator_v<Projection, Iterator, Compare>
        auto operator()(Iterator, Sentinel, Compare={}, Projection={}) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};

    struct projection_sorter:
        cppsort::sorter_facade<projection_sorter_impl>
    {};

    struct comparison_projection_sorter:
        cppsort::sorter_facade<comparison_projection_sorter_impl>
    {};
}

TEST_CASE( "sorter_facade miscellaneous checks",
           "[sorter_facade][comparison][projection][sentinel]" )
{
    // Some checks to make sure that sorter_facade always
    // forwards the value correctly in the most common cases

    using wrapper = generic_wrapper<int>;

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;

    auto vec_begin = vec.begin();
    auto vec_end = vec.end();
    auto sentinel_end = end_sentinel(vec_end);

    SECTION( "with comparison only" )
    {
        CHECK( comparison_sorter{}(vec, std::less{}) );
        CHECK( comparison_sorter{}(vec_begin, vec_end, std::less{}) );
        CHECK( comparison_sorter{}(vec_begin, sentinel_end, std::less{}) );

        CHECK( comparison_sorter{}(vec, std::greater{}) );
        CHECK( comparison_sorter{}(vec_begin, vec_end, std::greater{}) );
        CHECK( comparison_sorter{}(vec_begin, sentinel_end, std::greater{}) );
    }

    SECTION( "with projection only" )
    {
        CHECK( projection_sorter{}(vec, std::identity{}) );
        CHECK( projection_sorter{}(vec_begin, vec_end, std::identity{}) );
        CHECK( projection_sorter{}(vec_begin, sentinel_end, std::identity{}) );

        CHECK( projection_sorter{}(vec_wrap, &wrapper::value) );
        CHECK( projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value) );
        CHECK( projection_sorter{}(vec_wrap.begin(), end_sentinel(vec_wrap.end()), &wrapper::value) );
    }

    SECTION( "with both comparison and projection" )
    {
        CHECK( comparison_projection_sorter{}(vec, std::less{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, vec_end, std::less{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, sentinel_end, std::less{}) );

        CHECK( comparison_projection_sorter{}(vec, std::greater{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, vec_end, std::greater{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, sentinel_end, std::greater{}) );

        CHECK( comparison_projection_sorter{}(vec, std::identity{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, vec_end, std::identity{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, sentinel_end, std::identity{}) );

        CHECK( comparison_projection_sorter{}(vec_wrap, &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), end_sentinel(vec_wrap.end()), &wrapper::value) );

        CHECK( comparison_projection_sorter{}(vec, std::greater{}, std::identity{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, vec_end,
                                              std::greater{}, std::identity{}) );
        CHECK( comparison_projection_sorter{}(vec_begin, sentinel_end,
                                              std::greater{}, std::identity{}) );

        CHECK( comparison_projection_sorter{}(vec_wrap, std::greater{}, &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(),
                                              std::greater{}, &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), end_sentinel(vec_wrap.end()),
                                              std::greater{}, &wrapper::value) );
    }
}
