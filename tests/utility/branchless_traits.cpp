/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <string>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/partial_less.h>
#include <cpp-sort/comparators/total_less.h>
#include <cpp-sort/comparators/weak_less.h>
#include <cpp-sort/utility/branchless_traits.h>

TEST_CASE( "test that some specific comparisons are branchless",
           "[utility][branchless][comparison]" )
{
    using namespace cppsort::utility;

    SECTION( "standard library function objects" )
    {
        STATIC_CHECK( is_probably_branchless_comparison_v<std::less<int>, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::less<>, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::less<long double>, long double> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::ranges::less, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::ranges::less, long double> );

        STATIC_CHECK( is_probably_branchless_comparison_v<std::greater<int>, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::greater<>, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::greater<long double>, long double> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::ranges::greater, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::ranges::greater, long double> );

        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::less<std::string>, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::less<>, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::ranges::less, std::string> );

        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::greater<std::string>, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::greater<>, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<std::ranges::greater, std::string> );
    }

    SECTION( "partial/weak/less function objects" )
    {
        using partial_t = decltype(cppsort::partial_less);
        using weak_t = decltype(cppsort::weak_less);
        using total_t = decltype(cppsort::total_less);

        STATIC_CHECK( is_probably_branchless_comparison_v<partial_t, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<weak_t, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<total_t, int> );

        STATIC_CHECK( is_probably_branchless_comparison_v<partial_t, float> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<weak_t, float> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<total_t, float> );

        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<partial_t, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<weak_t, std::string> );
        STATIC_CHECK_FALSE( is_probably_branchless_comparison_v<total_t, std::string> );
    }

    SECTION( "cv-qualified and reference-qualified types" )
    {
        STATIC_CHECK( is_probably_branchless_comparison_v<std::less<>, const int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<const std::less<>&, int> );
        STATIC_CHECK( is_probably_branchless_comparison_v<const std::greater<>, int&&> );
        STATIC_CHECK( is_probably_branchless_comparison_v<std::ranges::greater, const int&> );
    }
}

TEST_CASE( "test that some specific projections are branchless",
           "[utility][branchless][projection]" )
{
    using namespace cppsort::utility;

    struct foobar
    {
        int foo;
        int bar() { return 0; }
    };

    STATIC_CHECK( is_probably_branchless_projection_v<std::identity, std::string> );

    STATIC_CHECK( is_probably_branchless_projection_v<decltype(&foobar::foo), foobar> );
    STATIC_CHECK_FALSE( is_probably_branchless_projection_v<decltype(&foobar::bar), foobar> );

#if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
    STATIC_CHECK( is_probably_branchless_projection_v<decltype(std::mem_fn(&foobar::foo)), foobar> );
#endif
    STATIC_CHECK_FALSE( is_probably_branchless_projection_v<decltype(std::mem_fn(&foobar::bar)), foobar> );
}
