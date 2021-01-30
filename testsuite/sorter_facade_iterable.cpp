/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <iterator>
#include <type_traits>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/wrapper.h>

namespace
{
    enum struct call
    {
        iterator,
        iterable
    };

    struct comparison_sorter_impl
    {
        template<typename Iterator, typename Compare=std::less<>>
        auto operator()(Iterator, Iterator, Compare={}) const
            -> call
        {
            return call::iterator;
        }

        template<typename Iterable, typename Compare=std::less<>>
        auto operator()(Iterable&, Compare={}) const
            -> call
        {
            return call::iterable;
        }
    };

    struct projection_sorter_impl
    {
        template<typename Iterator, typename Projection=cppsort::utility::identity>
        auto operator()(Iterator, Iterator, Projection={}) const
            -> call
        {
            return call::iterator;
        }

        template<typename Iterable, typename Projection=cppsort::utility::identity>
        auto operator()(Iterable&, Projection={}) const
            -> call
        {
            return call::iterable;
        }
    };

    struct comparison_projection_sorter_impl
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
            -> call
        {
            return call::iterator;
        }

        template<
            typename Iterable,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_v<
                Projection, Iterable, Compare
            >>
        >
        auto operator()(Iterable&, Compare={}, Projection={}) const
            -> call
        {
            return call::iterable;
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

TEST_CASE( "sorter_facade with sorters overloaded for iterables",
           "[sorter_facade][compare][projection]" )
{
    // Some sorters can optimize the computations a bit by adding
    // overloaded operator() that take a full iteratable instead
    // of a pair of iterators. We need to make sure that these
    // optimizations work too.

    using wrapper = generic_wrapper<int>;

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;

    SECTION( "with comparison only" )
    {
        call res1 = comparison_sorter{}(vec, std::less<>{});
        CHECK( res1 == call::iterable );
        call res2 = comparison_sorter{}(std::begin(vec), std::end(vec), std::less<>{});
        CHECK( res2 == call::iterator );

        call res3 = comparison_sorter{}(vec, std::greater<>{});
        CHECK( res3 == call::iterable );
        call res4 = comparison_sorter{}(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( res4 == call::iterator );
    }

    SECTION( "with projection only" )
    {
        call res1 = projection_sorter{}(vec, cppsort::utility::identity{});
        CHECK( res1 == call::iterable );
        call res2 = projection_sorter{}(std::begin(vec), std::end(vec), cppsort::utility::identity{});
        CHECK( res2 == call::iterator );

        call res3 = projection_sorter{}(vec_wrap, &wrapper::value);
        CHECK( res3 == call::iterable );
        call res4 = projection_sorter{}(std::begin(vec_wrap), std::end(vec_wrap), &wrapper::value);
        CHECK( res4 == call::iterator );
    }

    SECTION( "with both comparison and projection" )
    {
        call res1 = comparison_projection_sorter{}(vec, std::less<>{});
        CHECK( res1 == call::iterable );
        call res2 = comparison_projection_sorter{}(std::begin(vec), std::end(vec), std::less<>{});
        CHECK( res2 == call::iterator );

        call res3 = comparison_projection_sorter{}(vec, std::greater<>{});
        CHECK( res3 == call::iterable );
        call res4 = comparison_projection_sorter{}(std::begin(vec), std::end(vec), std::greater<>{});
        CHECK( res4 == call::iterator );

        call res5 = comparison_projection_sorter{}(vec, cppsort::utility::identity{});
        CHECK( res5 == call::iterable );
        call res6 = comparison_projection_sorter{}(std::begin(vec), std::end(vec),
                                                   cppsort::utility::identity{});
        CHECK( res6 == call::iterator );

        call res7 = comparison_projection_sorter{}(vec_wrap, &wrapper::value);
        CHECK( res7 == call::iterable );
        call res8 = comparison_projection_sorter{}(std::begin(vec_wrap), std::end(vec_wrap),
                                                   &wrapper::value);
        CHECK( res8 == call::iterator );

        call res9 = comparison_projection_sorter{}(vec, std::greater<>{}, cppsort::utility::identity{});
        CHECK( res9 == call::iterable );
        call res10 = comparison_projection_sorter{}(std::begin(vec), std::end(vec),
                                                    std::greater<>{}, cppsort::utility::identity{});
        CHECK( res10 == call::iterator );

        call res11 = comparison_projection_sorter{}(vec_wrap, std::greater<>{}, &wrapper::value);
        CHECK( res11 == call::iterable );
        call res12 = comparison_projection_sorter{}(std::begin(vec_wrap), std::end(vec_wrap),
                                                    std::greater<>{}, &wrapper::value);
        CHECK( res12 == call::iterator );
    }
}
