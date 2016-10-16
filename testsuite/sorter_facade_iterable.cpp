/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <functional>
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

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

    struct wrapper { int value; };

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;

    SECTION( "with comparison only" )
    {
        call res1 = cppsort::sort(comparison_sorter{}, vec, std::less<>{});
        CHECK( res1 == call::iterable );
        call res2 = cppsort::sort(comparison_sorter{}, std::begin(vec), std::end(vec),
                                  std::less<>{});
        CHECK( res2 == call::iterator );

        call res3 = cppsort::sort(comparison_sorter{}, vec, std::greater<>{});
        CHECK( res3 == call::iterable );
        call res4 = cppsort::sort(comparison_sorter{}, std::begin(vec), std::end(vec),
                                  std::greater<>{});
        CHECK( res4 == call::iterator );
    }

    SECTION( "with projection only" )
    {
        call res1 = cppsort::sort(projection_sorter{}, vec, cppsort::utility::identity{});
        CHECK( res1 == call::iterable );
        call res2 = cppsort::sort(projection_sorter{}, std::begin(vec), std::end(vec),
                                  cppsort::utility::identity{});
        CHECK( res2 == call::iterator );

        call res3 = cppsort::sort(projection_sorter{}, vec_wrap, &wrapper::value);
        CHECK( res3 == call::iterable );
        call res4 = cppsort::sort(projection_sorter{}, std::begin(vec_wrap), std::end(vec_wrap),
                                  &wrapper::value);
        CHECK( res4 == call::iterator );
    }

    SECTION( "with both comparison and projection" )
    {
        call res1 = cppsort::sort(comparison_projection_sorter{}, vec, std::less<>{});
        CHECK( res1 == call::iterable );
        call res2 = cppsort::sort(comparison_projection_sorter{},
                                  std::begin(vec), std::end(vec),
                                  std::less<>{});
        CHECK( res2 == call::iterator );

        call res3 = cppsort::sort(comparison_projection_sorter{}, vec, std::greater<>{});
        CHECK( res3 == call::iterable );
        call res4 = cppsort::sort(comparison_projection_sorter{},
                                  std::begin(vec), std::end(vec),
                                  std::greater<>{});
        CHECK( res4 == call::iterator );

        call res5 = cppsort::sort(comparison_projection_sorter{}, vec, cppsort::utility::identity{});
        CHECK( res5 == call::iterable );
        call res6 = cppsort::sort(comparison_projection_sorter{},
                                  std::begin(vec), std::end(vec),
                                  cppsort::utility::identity{});
        CHECK( res6 == call::iterator );

        call res7 = cppsort::sort(comparison_projection_sorter{}, vec_wrap, &wrapper::value);
        CHECK( res7 == call::iterable );
        call res8 = cppsort::sort(comparison_projection_sorter{},
                                  std::begin(vec_wrap), std::end(vec_wrap),
                                  &wrapper::value);
        CHECK( res8 == call::iterator );

        call res9 = cppsort::sort(comparison_projection_sorter{}, vec,
                                  std::greater<>{}, cppsort::utility::identity{});
        CHECK( res9 == call::iterable );
        call res10 = cppsort::sort(comparison_projection_sorter{},
                                   std::begin(vec), std::end(vec),
                                   std::greater<>{}, cppsort::utility::identity{});
        CHECK( res10 == call::iterator );

        call res11 = cppsort::sort(comparison_projection_sorter{}, vec_wrap,
                                   std::greater<>{}, &wrapper::value);
        CHECK( res11 == call::iterable );
        call res12 = cppsort::sort(comparison_projection_sorter{},
                                   std::begin(vec_wrap), std::end(vec_wrap),
                                   std::greater<>{}, &wrapper::value);
        CHECK( res12 == call::iterator );
    }
}
