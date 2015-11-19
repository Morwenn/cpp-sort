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
    struct comparison_sorter_impl
    {
        template<typename Iterator, typename Compare=std::less<>>
        auto operator()(Iterator, Iterator, Compare={}) const
            -> bool
        {
            return true;
        }
    };

    struct projection_sorter_impl
    {
        template<typename Iterator, typename Projection=cppsort::utility::identity>
        auto operator()(Iterator, Iterator, Projection={}) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator, Iterator, Compare={}, Projection={}) const
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
           "[sorter_facade][compare][projection]" )
{
    // Some checks to make sure that sorter_facade always
    // forwards the value correctly in the most common cases

    struct wrapper { int value; };

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;

    SECTION( "with comparison only" )
    {
        CHECK( cppsort::sort(vec, comparison_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_sorter{}, std::less<>{}) );

        CHECK( cppsort::sort(vec, comparison_sorter{}, std::greater<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_sorter{}, std::greater<>{}) );
    }

    SECTION( "with projection only" )
    {
        CHECK( cppsort::sort(vec, projection_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             projection_sorter{}, cppsort::utility::identity{}) );

        CHECK( cppsort::sort(vec_wrap, projection_sorter{}, &wrapper::value) );
        CHECK( cppsort::sort(std::begin(vec_wrap), std::end(vec_wrap),
                             projection_sorter{}, &wrapper::value) );
    }

    SECTION( "with both comparison and projection" )
    {
        CHECK( cppsort::sort(vec, comparison_projection_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_projection_sorter{}, std::less<>{}) );

        CHECK( cppsort::sort(vec, comparison_projection_sorter{}, std::greater<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_projection_sorter{}, std::greater<>{}) );

        CHECK( cppsort::sort(vec, comparison_projection_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_projection_sorter{}, cppsort::utility::identity{}) );

        CHECK( cppsort::sort(vec_wrap, comparison_projection_sorter{}, &wrapper::value) );
        CHECK( cppsort::sort(std::begin(vec_wrap), std::end(vec_wrap),
                             comparison_projection_sorter{}, &wrapper::value) );

        CHECK( cppsort::sort(vec, comparison_projection_sorter{},
                             std::greater<>{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec),
                             comparison_projection_sorter{},
                             std::greater<>{}, cppsort::utility::identity{}) );

        CHECK( cppsort::sort(vec_wrap, comparison_projection_sorter{},
                             std::greater<>{}, &wrapper::value) );
        CHECK( cppsort::sort(std::begin(vec_wrap), std::end(vec_wrap),
                             comparison_projection_sorter{},
                             std::greater<>{}, &wrapper::value) );
    }
}
