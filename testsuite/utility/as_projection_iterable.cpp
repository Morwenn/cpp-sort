/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#include <algorithm>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

namespace
{
    struct tricky_function
    {
        template<typename T, typename U>
        auto operator()(T&& lhs, U&& rhs)
            noexcept(noexcept(std::forward<T>(lhs) > std::forward<U>(rhs)))
            -> decltype(std::forward<T>(lhs) > std::forward<U>(rhs))
        {
            // Compare values in reverse order
            return std::forward<T>(lhs) > std::forward<U>(rhs);
        }

        template<typename T>
        auto operator()(T&& value) const noexcept
            -> decltype(std::forward<T>(value))
        {
            return std::forward<T>(value);
        }
    };

    enum struct call
    {
        iterator,
        iterable
    };

    struct comparison_sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                cppsort::utility::identity, Iterator, Compare
            >>
        >
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> call
        {
            cppsort::sort(first, last, compare);
            return call::iterator;
        }

        template<
            typename Iterable,
            typename Compare = std::less<>,
            typename = std::enable_if_t<cppsort::is_projection_v<
                cppsort::utility::identity, Iterable, Compare
            >>
        >
        auto operator()(Iterable& iterable, Compare compare={}) const
            -> call
        {
            cppsort::sort(iterable, compare);
            return call::iterable;
        }
    };

    struct projection_sorter_impl
    {
        template<
            typename Iterator,
            typename Projection=cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, Iterator
            >>
        >
        auto operator()(Iterator first, Iterator last, Projection projection={}) const
            -> call
        {
            // Use as_projection to make an actual projection-only sorter
            cppsort::sort(first, last, cppsort::utility::as_projection(projection));
            return call::iterator;
        }

        template<
            typename Iterable,
            typename Projection=cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_v<
                Projection, Iterable
            >>
        >
        auto operator()(Iterable& iterable, Projection projection={}) const
            -> call
        {
            // Use as_projection to make an actual projection-only sorter
            cppsort::sort(iterable, cppsort::utility::as_projection(projection));
            return call::iterable;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};

    struct projection_sorter:
        cppsort::sorter_facade<projection_sorter_impl>
    {};
}

TEST_CASE( "sorter_facade with sorters overloaded for iterables and mixed comparison/projection",
           "[sorter_facade][compare][projection][as_projection]" )
{
    // Test the intersection between mixed comparison/projection functions,
    // as_projection, as_comparison and some additional sorter_facade
    // overloads

    struct wrapper { int value; };

    // Collection to sort
    std::vector<int> collection(100);
    std::iota(std::begin(collection), std::end(collection), 0);
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);
    auto vec = collection;

    tricky_function func;
    comparison_sorter comp_sort;
    projection_sorter proj_sort;

    SECTION( "comparison_sorter" )
    {
        vec = collection;
        auto res1 = cppsort::sort(comp_sort, vec, func);
        CHECK( res1 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res2 = cppsort::sort(comp_sort, std::begin(vec), std::end(vec), func);
        CHECK( res2 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res3 = cppsort::sort(comp_sort, vec, cppsort::utility::as_comparison(func));
        CHECK( res3 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res4 = cppsort::sort(comp_sort, std::begin(vec), std::end(vec),
                                  cppsort::utility::as_comparison(func));
        CHECK( res4 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res5 = cppsort::sort(comp_sort, vec, cppsort::utility::as_projection(func));
        CHECK( res5 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        auto res6 = cppsort::sort(comp_sort, std::begin(vec), std::end(vec),
                                  cppsort::utility::as_projection(func));
        CHECK( res6 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        auto res7 = cppsort::sort(comp_sort, vec, func,
                                  cppsort::utility::as_projection(func));
        CHECK( res7 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res8 = cppsort::sort(comp_sort, std::begin(vec), std::end(vec), func,
                                  cppsort::utility::as_projection(func));
        CHECK( res8 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res9 = cppsort::sort(comp_sort, vec, cppsort::utility::as_comparison(func),
                                                  cppsort::utility::as_projection(func));
        CHECK( res9 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );

        vec = collection;
        auto res10 = cppsort::sort(comp_sort, std::begin(vec), std::end(vec),
                                   cppsort::utility::as_comparison(func),
                                   cppsort::utility::as_projection(func));
        CHECK( res10 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec), std::greater<>{}) );
    }

    SECTION( "projection_sorter" )
    {
        vec = collection;
        auto res1 = cppsort::sort(proj_sort, vec, cppsort::utility::as_projection(func));
        CHECK( res1 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        auto res2 = cppsort::sort(proj_sort, std::begin(vec), std::end(vec),
                                  cppsort::utility::as_projection(func));
        CHECK( res2 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        auto res3 = cppsort::sort(proj_sort, vec, func);
        CHECK( res3 == call::iterable );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );

        vec = collection;
        auto res4 = cppsort::sort(proj_sort, std::begin(vec), std::end(vec), func);
        CHECK( res4 == call::iterator );
        CHECK( std::is_sorted(std::begin(vec), std::end(vec)) );
    }
}
