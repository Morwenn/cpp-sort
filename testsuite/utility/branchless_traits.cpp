/*
 * Copyright (c) 2017-2018 Morwenn
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
        CHECK(( is_probably_branchless_comparison<std::less<int>, int>::value ));
        CHECK(( is_probably_branchless_comparison<std::less<>, int>::value ));
        CHECK(( is_probably_branchless_comparison<std::less<long double>, long double>::value ));

        CHECK(( is_probably_branchless_comparison<std::greater<int>, int>::value ));
        CHECK(( is_probably_branchless_comparison<std::greater<>, int>::value ));
        CHECK(( is_probably_branchless_comparison<std::greater<long double>, long double>::value ));

        CHECK_FALSE(( is_probably_branchless_comparison<std::less<std::string>, std::string>::value ));
        CHECK_FALSE(( is_probably_branchless_comparison<std::less<>, std::string>::value ));
    }

    SECTION( "partial/weak/less function objects" )
    {
        using partial_t = decltype(cppsort::partial_less);
        using weak_t = decltype(cppsort::weak_less);
        using total_t = decltype(cppsort::total_less);

        CHECK(( is_probably_branchless_comparison<partial_t, int>::value ));
        CHECK(( is_probably_branchless_comparison<weak_t, int>::value ));
        CHECK(( is_probably_branchless_comparison<total_t, int>::value ));

        CHECK(( is_probably_branchless_comparison<partial_t, float>::value ));
        CHECK_FALSE(( is_probably_branchless_comparison<weak_t, float>::value ));
        CHECK_FALSE(( is_probably_branchless_comparison<total_t, float>::value ));

        CHECK_FALSE(( is_probably_branchless_comparison<partial_t, std::string>::value ));
        CHECK_FALSE(( is_probably_branchless_comparison<weak_t, std::string>::value ));
        CHECK_FALSE(( is_probably_branchless_comparison<total_t, std::string>::value ));
    }

    SECTION( "cv-qualified and reference-qualified types" )
    {
        CHECK(( is_probably_branchless_comparison<std::less<>, const int>::value ));
        CHECK(( is_probably_branchless_comparison<const std::less<>&, int>::value ));
        CHECK(( is_probably_branchless_comparison<const std::greater<>, int&&>::value ));
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

    CHECK(( is_probably_branchless_projection<identity, std::string>::value ));

    CHECK(( is_probably_branchless_projection<decltype(&foobar::foo), foobar>::value ));
    CHECK_FALSE(( is_probably_branchless_projection<decltype(&foobar::bar), foobar>::value ));
}
