/*
 * Copyright (c) 2020-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/spin_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_wrapper<int>;

namespace
{
    struct proj1:
        cppsort::utility::projection_base
    {
        auto operator()(int value) const
            -> int
        {
            return -value;
        }
    };

    struct proj2:
        cppsort::utility::projection_base
    {
        auto operator()(int value)
            -> int
        {
            return -value;
        }
    };
}

TEST_CASE( "Pipe a projection_base and function pointer",
           "[utility][projection_base]" )
{
    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    proj1 projection1;
    proj2 projection2;

    SECTION( "const projection" )
    {
        cppsort::spin_sort(vec, &wrapper::value | projection1);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "chained const projection" )
    {
        cppsort::spin_sort(vec, &wrapper::value | projection1 | std::negate<>{});
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }

    SECTION( "non-const projection" )
    {
        cppsort::spin_sort(vec, &wrapper::value | projection2);
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
    }

    SECTION( "chained non-const projection" )
    {
        cppsort::spin_sort(vec, &wrapper::value | projection2 | std::negate<>{});
        CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::less<>{}, &wrapper::value) );
    }
}


TEST_CASE( "Pipe a projection_base several times",
           "[utility][projection_base]" )
{
    std::vector<int> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80, -38);
    auto vec2 = vec;

    proj1 projection1;
    proj2 projection2;

    cppsort::spin_sort(vec, projection1 | projection2);
    CHECK( std::is_sorted(vec.begin(), vec.end()) );

    cppsort::spin_sort(vec2, projection2 | projection1 | projection2);
    CHECK( std::is_sorted(vec2.begin(), vec2.end(), std::greater<>{}) );
}

TEST_CASE( "Pipe a projection with as_projection",
           "[utility][projection_base]" )
{
    std::vector<wrapper> vec;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(vec), 80);

    auto projection1 = cppsort::utility::as_projection(&wrapper::value);
    auto projection2 = proj1();

    // Basic check
    cppsort::spin_sort(vec, projection1 | projection2);
    CHECK( helpers::is_sorted(vec.begin(), vec.end(), std::greater<>{}, &wrapper::value) );
}

TEST_CASE( "Pipe utility::identity with utility::identity",
           "[utility][projection_base]" )
{
    std::vector<int> vec(15, 42);
    cppsort::spin_sort(vec, cppsort::utility::identity{} | cppsort::utility::identity{});
    CHECK( true );
}
