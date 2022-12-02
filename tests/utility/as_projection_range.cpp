/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>

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
        range
    };

    struct comparison_sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>
        >
            requires cppsort::is_projection_iterator_v<std::identity, Iterator, Compare>
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> call
        {
            cppsort::selection_sort(first, last, compare);
            return call::iterator;
        }

        template<
            typename Range,
            typename Compare = std::less<>
        >
            requires cppsort::is_projection_v<std::identity, Range, Compare>
        auto operator()(Range& range, Compare compare={}) const
            -> call
        {
            cppsort::selection_sort(range, compare);
            return call::range;
        }
    };

    struct projection_sorter_impl
    {
        template<
            typename Iterator,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_iterator_v<Projection, Iterator>
        auto operator()(Iterator first, Iterator last, Projection projection={}) const
            -> call
        {
            // Use as_projection to make an actual projection-only sorter
            cppsort::selection_sort(first, last, cppsort::utility::as_projection(projection));
            return call::iterator;
        }

        template<
            typename Range,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_v<Projection, Range>
        auto operator()(Range& range, Projection projection={}) const
            -> call
        {
            // Use as_projection to make an actual projection-only sorter
            cppsort::selection_sort(range, cppsort::utility::as_projection(projection));
            return call::range;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};

    struct projection_sorter:
        cppsort::sorter_facade<projection_sorter_impl>
    {};
}

TEST_CASE( "sorter_facade with sorters overloaded for ranges and mixed comparison/projection",
           "[sorter_facade][comparison][projection][as_projection]" )
{
    // Test the intersection between mixed comparison/projection functions,
    // as_projection, as_comparison and some additional sorter_facade
    // overloads

    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 100);
    auto vec = collection;

    tricky_function func;
    comparison_sorter comp_sort;
    projection_sorter proj_sort;

    SECTION( "comparison_sorter" )
    {
        auto res1 = comp_sort(vec, func);
        CHECK( res1 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res2 = comp_sort(vec.begin(), vec.end(), func);
        CHECK( res2 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res3 = comp_sort(vec, cppsort::utility::as_comparison(func));
        CHECK( res3 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res4 = comp_sort(vec.begin(), vec.end(), cppsort::utility::as_comparison(func));
        CHECK( res4 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res5 = comp_sort(vec, cppsort::utility::as_projection(func));
        CHECK( res5 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        auto res6 = comp_sort(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( res6 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        auto res7 = comp_sort(vec, func, cppsort::utility::as_projection(func));
        CHECK( res7 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res8 = comp_sort(vec.begin(), vec.end(), func,
                              cppsort::utility::as_projection(func));
        CHECK( res8 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res9 = comp_sort(vec, cppsort::utility::as_comparison(func),
                              cppsort::utility::as_projection(func));
        CHECK( res9 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        auto res10 = comp_sort(vec.begin(), vec.end(),
                               cppsort::utility::as_comparison(func),
                               cppsort::utility::as_projection(func));
        CHECK( res10 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "projection_sorter" )
    {
        auto res1 = proj_sort(vec, cppsort::utility::as_projection(func));
        CHECK( res1 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        auto res2 = proj_sort(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( res2 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        auto res3 = proj_sort(vec, func);
        CHECK( res3 == call::range );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        auto res4 = proj_sort(vec.begin(), vec.end(), func);
        CHECK( res4 == call::iterator );
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}
