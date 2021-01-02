/*
 * Copyright (c) 2017-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <string>
#include <catch2/catch.hpp>
#include <cpp-sort/comparators/partial_less.h>
#include <cpp-sort/comparators/total_less.h>
#include <cpp-sort/comparators/weak_less.h>
#include <cpp-sort/utility/branchless_traits.h>
#include <cpp-sort/utility/functional.h>

TEST_CASE( "test that some specific comparisons are branchless",
           "[utility][branchless][comparison]" )
{
    using namespace cppsort::utility;

    SECTION( "standard library function objects" )
    {
        CHECK(( is_probably_branchless_comparison_v<std::less<int>, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::less<>, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::less<long double>, long double> ));
#ifdef __cpp_lib_ranges
        CHECK(( is_probably_branchless_comparison_v<std::ranges::less, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::ranges::less, long double> ));
#endif

        CHECK(( is_probably_branchless_comparison_v<std::greater<int>, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::greater<>, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::greater<long double>, long double> ));
#ifdef __cpp_lib_ranges
        CHECK(( is_probably_branchless_comparison_v<std::ranges::greater, int> ));
        CHECK(( is_probably_branchless_comparison_v<std::ranges::greater, long double> ));
#endif

        CHECK_FALSE(( is_probably_branchless_comparison_v<std::less<std::string>, std::string> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<std::less<>, std::string> ));
#ifdef __cpp_lib_ranges
        CHECK_FALSE(( is_probably_branchless_comparison_v<std::ranges::less, std::string> ));
#endif

        CHECK_FALSE(( is_probably_branchless_comparison_v<std::greater<std::string>, std::string> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<std::greater<>, std::string> ));
#ifdef __cpp_lib_ranges
        CHECK_FALSE(( is_probably_branchless_comparison_v<std::ranges::greater, std::string> ));
#endif
    }

    SECTION( "partial/weak/less function objects" )
    {
        using partial_t = decltype(cppsort::partial_less);
        using weak_t = decltype(cppsort::weak_less);
        using total_t = decltype(cppsort::total_less);

        CHECK(( is_probably_branchless_comparison_v<partial_t, int> ));
        CHECK(( is_probably_branchless_comparison_v<weak_t, int> ));
        CHECK(( is_probably_branchless_comparison_v<total_t, int> ));

        CHECK(( is_probably_branchless_comparison_v<partial_t, float> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<weak_t, float> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<total_t, float> ));

        CHECK_FALSE(( is_probably_branchless_comparison_v<partial_t, std::string> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<weak_t, std::string> ));
        CHECK_FALSE(( is_probably_branchless_comparison_v<total_t, std::string> ));
    }

    SECTION( "cv-qualified and reference-qualified types" )
    {
        CHECK(( is_probably_branchless_comparison_v<std::less<>, const int> ));
        CHECK(( is_probably_branchless_comparison_v<const std::less<>&, int> ));
        CHECK(( is_probably_branchless_comparison_v<const std::greater<>, int&&> ));
#ifdef __cpp_lib_ranges
        CHECK(( is_probably_branchless_comparison_v<std::ranges::greater, const int&> ));
#endif
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

    CHECK(( is_probably_branchless_projection_v<identity, std::string> ));
#if CPPSORT_STD_IDENTITY_AVAILABLE
    CHECK(( is_probably_branchless_projection_v<std::identity, std::string> ));
#endif

    CHECK(( is_probably_branchless_projection_v<decltype(&foobar::foo), foobar> ));
    CHECK_FALSE(( is_probably_branchless_projection_v<decltype(&foobar::bar), foobar> ));

#if defined(__GLIBCXX__) || defined(_LIBCPP_VERSION)
    CHECK(( is_probably_branchless_projection_v<decltype(std::mem_fn(&foobar::foo)), foobar> ));
#endif
    CHECK_FALSE(( is_probably_branchless_projection_v<decltype(std::mem_fn(&foobar::bar)), foobar> ));
}
