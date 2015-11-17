/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/identity.h>

namespace
{
    using namespace cppsort;

    enum struct call
    {
        iterator_compare,
        iterable_compare,
        iterator_projection,
        iterable_projection
    };

    struct comparison_sorter_impl
    {
        template<typename Iterator, typename Compare=std::less<>>
        auto operator()(Iterator, Iterator, Compare={}) const
            -> call
        {
            return call::iterator_compare;
        }

        template<typename Iterable, typename Compare=std::less<>>
        auto operator()(Iterable&, Compare={}) const
            -> call
        {
            return call::iterable_compare;
        }
    };

    struct projection_sorter_impl
    {
        template<typename Iterator, typename Projection=utility::identity>
        auto operator()(Iterator, Iterator, Projection={}) const
            -> call
        {
            return call::iterator_projection;
        }

        template<typename Iterable, typename Projection=utility::identity>
        auto operator()(Iterable&, Projection={}) const
            -> call
        {
            return call::iterable_projection;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection=utility::identity
        >
        auto operator()(Iterator, Iterator, Compare={}, Projection={}) const
            -> call
        {
            return call::iterator_projection;
        }

        template<
            typename Iterable,
            typename Compare = std::less<>,
            typename Projection=utility::identity
        >
        auto operator()(Iterable&, Compare={}, Projection={}) const
            -> call
        {
            return call::iterable_projection;
        }
    };

    struct comparison_sorter:
        sorter_facade<comparison_sorter_impl>
    {};

    struct projection_sorter:
        sorter_facade<projection_sorter_impl>
    {};

    struct comparison_projection_sorter:
        sorter_facade<comparison_projection_sorter_impl>
    {};
}

TEST_CASE( "miscellaneous sorter_facade checks",
           "[sorter_facade][compare][projection]" )
{
    // Miscellaneous sorter_facade checks to make sure
    // that no operator() ambiguity is introduced

    SECTION( "conditional iterable operator() overload for comparison" )
    {
        std::vector<int> vec(3);

        call res1 = cppsort::sort(vec, comparison_sorter{}, std::less<>{});
        CHECK( res1 == call::iterable_compare );
        call res2 = cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, std::less<>{});
        CHECK( res2 == call::iterator_compare );

        call res3 = cppsort::sort(vec, comparison_sorter{}, std::greater<>{});
        CHECK( res3 == call::iterable_compare );
        call res4 = cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, std::greater<>{});
        CHECK( res4 == call::iterator_compare );
    }

    SECTION( "conditional iterable operator() overload for projection" )
    {
        struct wrapper { int value; };
        std::vector<wrapper> vec(3);

        call res1 = cppsort::sort(vec, projection_sorter{}, &wrapper::value);
        CHECK( res1 == call::iterable_projection );
        call res2 = cppsort::sort(std::begin(vec), std::end(vec), projection_sorter{}, &wrapper::value);
        CHECK( res2 == call::iterator_projection );
    }

    SECTION( "conditional iterable operator() when both are here" )
    {
        std::vector<int> vec;

        call res1 = cppsort::sort(vec, comparison_projection_sorter{}, std::less<>{});
        CHECK( res1 == call::iterable_projection );
        call res2 = cppsort::sort(std::begin(vec), std::end(vec), comparison_projection_sorter{}, std::less<>{});
        CHECK( res2 == call::iterator_projection );

        call res3 = cppsort::sort(vec, comparison_projection_sorter{}, std::greater<>{});
        CHECK( res3 == call::iterable_projection );
        call res4 = cppsort::sort(std::begin(vec), std::end(vec), comparison_projection_sorter{}, std::greater<>{});
        CHECK( res4 == call::iterator_projection );

        struct wrapper { int value; };
        std::vector<wrapper> vec2(3);

        call res5 = cppsort::sort(vec2, comparison_projection_sorter{}, &wrapper::value);
        CHECK( res5 == call::iterable_projection );
        call res6 = cppsort::sort(std::begin(vec2), std::end(vec2), comparison_projection_sorter{}, &wrapper::value);
        CHECK( res6 == call::iterator_projection );
    }
}
