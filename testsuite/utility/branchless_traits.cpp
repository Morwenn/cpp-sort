/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
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
#include <string>
#include <catch.hpp>
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
