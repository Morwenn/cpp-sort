/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/old_default_sorter.h>

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
}

TEST_CASE( "try mixed comparison/projection function object",
           "[utility][as_projection]" )
{
    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 100);

    tricky_function func;
    old_default_sorter sorter;
    cppsort::stable_adapter<old_default_sorter> stable_sorter;

    SECTION( "without an untransformed function" )
    {
        auto vec = collection;
        sorter(vec, func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        sorter(vec.begin(), vec.end(), func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec, func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec, func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec.begin(), vec.end(), func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        stable_sorter(vec, func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        stable_sorter(vec.begin(), vec.end(), func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec, func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec.begin(), vec.end(), func);
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "with a function wrapped in as_projection" )
    {
        auto vec = collection;
        sorter(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        sorter(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        cppsort::pdq_sort(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        cppsort::pdq_sort(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        stable_sorter(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        stable_sorter(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec, cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec.begin(), vec.end(), cppsort::utility::as_projection(func));
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "with a function wrapped in as_comparison" )
    {
        auto vec = collection;
        sorter(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        sorter(vec.begin(), vec.end(), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::pdq_sort(vec.begin(), vec.end(), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        stable_sorter(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        stable_sorter(vec.begin(), vec.end(), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec, cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        vec = collection;
        cppsort::stable_adapter<cppsort::pdq_sorter>{}(vec.begin(), vec.end(), cppsort::utility::as_comparison(func));
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }
}
