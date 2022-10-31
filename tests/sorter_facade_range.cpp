/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <type_traits>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <testing-tools/end_sentinel.h>
#include <testing-tools/wrapper.h>

namespace
{
    enum struct call
    {
        iterator,
        range
    };

    struct comparison_sorter_impl
    {
        template<
            cppsort::mstd::forward_iterator Iterator,
            cppsort::mstd::sentinel_for<Iterator> Sentinel,
            typename Compare = std::less<>
        >
        auto operator()(Iterator, Sentinel, Compare={}) const
            -> call
        {
            return call::iterator;
        }

        template<
            cppsort::mstd::forward_range Range,
            typename Compare = std::less<>
        >
        auto operator()(Range&, Compare={}) const
            -> call
        {
            return call::range;
        }
    };

    struct projection_sorter_impl
    {
        template<
            cppsort::mstd::forward_iterator Iterator,
            cppsort::mstd::sentinel_for<Iterator> Sentinel,
            typename Projection = std::identity
        >
        auto operator()(Iterator, Sentinel, Projection={}) const
            -> call
        {
            return call::iterator;
        }

        template<
            cppsort::mstd::forward_range Range,
            typename Projection = std::identity
        >
        auto operator()(Range&, Projection={}) const
            -> call
        {
            return call::range;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<
            cppsort::mstd::forward_iterator Iterator,
            cppsort::mstd::sentinel_for<Iterator> Sentinel,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator, Sentinel, Compare={}, Projection={}) const
            -> call
        {
            return call::iterator;
        }

        template<
            cppsort::mstd::forward_range Range,
            typename Compare = std::less<>,
            typename Projection = std::identity,
            typename = std::enable_if_t<cppsort::is_projection_v<
                Projection, Range, Compare
            >>
        >
        auto operator()(Range&, Compare={}, Projection={}) const
            -> call
        {
            return call::range;
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

TEST_CASE( "sorter_facade with sorters overloaded for ranges",
           "[sorter_facade][comparison][projection]" )
{
    // Some sorters can optimize the computations a bit by adding
    // overloaded operator() that take a full range instead of an
    // iterator/sentinel pair. We ensure that such overloads are
    // preferred over iterator/sentinel ones where it makes sense.

    using wrapper = generic_wrapper<int>;

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;
    end_sentinel_range<std::vector<int>::iterator> sentinel_vec(vec);
    end_sentinel_range<std::vector<wrapper>::iterator> sentinel_vec_wrap(vec_wrap);

    SECTION( "with comparison only" )
    {
        call res1 = comparison_sorter{}(vec, std::less<>{});
        CHECK( res1 == call::range );
        call res2 = comparison_sorter{}(vec.begin(), vec.end(), std::less<>{});
        CHECK( res2 == call::iterator );
        call res3 = comparison_sorter{}(sentinel_vec, std::greater<>{});
        CHECK( res3 == call::range );

        call res4 = comparison_sorter{}(vec, std::greater<>{});
        CHECK( res4 == call::range );
        call res5 = comparison_sorter{}(vec.begin(), vec.end(), std::greater<>{});
        CHECK( res5 == call::iterator );
        call res6 = comparison_sorter{}(sentinel_vec, std::greater<>{});
        CHECK( res6 == call::range );
    }

    SECTION( "with projection only" )
    {
        call res1 = projection_sorter{}(vec, std::identity{});
        CHECK( res1 == call::range );
        call res2 = projection_sorter{}(vec.begin(), vec.end(), std::identity{});
        CHECK( res2 == call::iterator );
        call res3 = projection_sorter{}(sentinel_vec, std::identity{});
        CHECK( res3 == call::range );

        call res4 = projection_sorter{}(vec_wrap, &wrapper::value);
        CHECK( res4 == call::range );
        call res5 = projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value);
        CHECK( res5 == call::iterator );
        call res6 = projection_sorter{}(sentinel_vec_wrap, &wrapper::value);
        CHECK( res6 == call::range );
    }

    SECTION( "with both comparison and projection" )
    {
        call res1 = comparison_projection_sorter{}(vec, std::less<>{});
        CHECK( res1 == call::range );
        call res2 = comparison_projection_sorter{}(vec.begin(), vec.end(), std::less<>{});
        CHECK( res2 == call::iterator );
        call res3 = comparison_projection_sorter{}(sentinel_vec, std::less<>{});
        CHECK( res3 == call::range );

        call res4 = comparison_projection_sorter{}(vec, std::greater<>{});
        CHECK( res4 == call::range );
        call res5 = comparison_projection_sorter{}(vec.begin(), vec.end(), std::greater<>{});
        CHECK( res5 == call::iterator );
        call res6 = comparison_projection_sorter{}(sentinel_vec, std::greater<>{});
        CHECK( res6 == call::range );

        call res7 = comparison_projection_sorter{}(vec, std::identity{});
        CHECK( res7 == call::range );
        call res8 = comparison_projection_sorter{}(vec.begin(), vec.end(),
                                                   std::identity{});
        CHECK( res8 == call::iterator );
        call res9 = comparison_projection_sorter{}(sentinel_vec, std::identity{});
        CHECK( res9 == call::range );

        call res10 = comparison_projection_sorter{}(vec_wrap, &wrapper::value);
        CHECK( res10 == call::range );
        call res11 = comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value);
        CHECK( res11 == call::iterator );
        call res12 = comparison_projection_sorter{}(sentinel_vec_wrap, &wrapper::value);
        CHECK( res12 == call::range );

        call res13 = comparison_projection_sorter{}(vec, std::greater<>{}, std::identity{});
        CHECK( res13 == call::range );
        call res14 = comparison_projection_sorter{}(vec.begin(), vec.end(),
                                                    std::greater<>{}, std::identity{});
        CHECK( res14 == call::iterator );
        call res15 = comparison_projection_sorter{}(sentinel_vec, std::greater<>{}, std::identity{});
        CHECK( res15 == call::range );

        call res16 = comparison_projection_sorter{}(vec_wrap, std::greater<>{}, &wrapper::value);
        CHECK( res16 == call::range );
        call res17 = comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(),
                                                    std::greater<>{}, &wrapper::value);
        CHECK( res17 == call::iterator );
        call res18 = comparison_projection_sorter{}(sentinel_vec_wrap, std::greater<>{}, &wrapper::value);
        CHECK( res18 == call::range );
    }
}
