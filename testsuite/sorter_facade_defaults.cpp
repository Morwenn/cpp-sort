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
        template<typename Iterator, typename Compare>
        auto operator()(Iterator, Iterator, Compare) const
            -> bool
        {
            return true;
        }

        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> bool
        {
            return false;
        }
    };

    struct non_comparison_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> bool
        {
            return true;
        }
    };

    struct non_comparison_iterable_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> bool
        {
            return true;
        }

        template<typename Iterable>
        auto operator()(Iterable&) const
            -> bool
        {
            return false;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<typename Iterator, typename Compare, typename Projection>
        auto operator()(Iterator, Iterator, Compare, Projection) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};

    struct non_comparison_sorter:
        cppsort::sorter_facade<non_comparison_sorter_impl>
    {};

    struct non_comparison_iterable_sorter:
        cppsort::sorter_facade<non_comparison_iterable_sorter_impl>
    {};

    struct comparison_projection_sorter:
        cppsort::sorter_facade<comparison_projection_sorter_impl>
    {};
}

TEST_CASE( "std::less<> forwarding to sorters",
           "[sorter_facade][compare]" )
{
    // Check that sorter_facade only creates the overloads for
    // std::less when the original sorter does not support
    // custom comparison functions

    // Make sure the iterator overload calls the operator() from
    // the sorter for iterators, and that the iterable overload
    // calls the original sorter's iterable operator() overload

    // Equivalent tests are done for the automatic overloads
    // of operator() for utility::identity

    // Vector to "sort"
    std::vector<int> vec(3);

    SECTION( "with std::less<>" )
    {
        CHECK( cppsort::sort(vec, comparison_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, std::less<>{}) );

        CHECK( cppsort::sort(vec, non_comparison_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), non_comparison_sorter{}, std::less<>{}) );

        CHECK( not cppsort::sort(vec, non_comparison_iterable_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), non_comparison_iterable_sorter{}, std::less<>{}) );
    }

    SECTION( "with utility::identity" )
    {
        CHECK( cppsort::sort(vec, comparison_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, cppsort::utility::identity{}) );

        CHECK( cppsort::sort(vec, non_comparison_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), non_comparison_sorter{}, cppsort::utility::identity{}) );

        CHECK( not cppsort::sort(vec, non_comparison_iterable_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), non_comparison_iterable_sorter{}, cppsort::utility::identity{}) );

        CHECK( cppsort::sort(vec, comparison_projection_sorter{}, cppsort::utility::identity{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), comparison_projection_sorter{}, cppsort::utility::identity{}) );
    }
}
