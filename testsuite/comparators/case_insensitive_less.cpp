/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#include <array>
#include <locale>
#include <string>
#include <catch2/catch.hpp>
#include <cpp-sort/comparators/case_insensitive_less.h>
#include <cpp-sort/refined.h>
#include <cpp-sort/sort.h>

namespace sub
{
    enum struct res_t
    {
        with_locale,
        without_locale
    };

    struct foo {};

    inline auto case_insensitive_less(const foo&, const foo&)
        -> res_t
    {
        return res_t::without_locale;
    }

    inline auto case_insensitive_less(const foo&, const foo&, const std::locale)
        -> res_t
    {
        return res_t::with_locale;
    }
}

TEST_CASE( "case-insensitive string comparison with case_insensitive_less" )
{
    std::array<std::string, 9> array = {
        "awry",
        "Banana",
        "greaTEr",
        "bounTy",
        "eXpected",
        "aROma",
        "excellent",
        "GreaT",
        "aROused"
    };

    std::array<std::string, 9> expected = {
        "aROma",
        "aROused",
        "awry",
        "Banana",
        "bounTy",
        "excellent",
        "eXpected",
        "GreaT",
        "greaTEr"
    };

    SECTION( "implicit global locale" )
    {
        cppsort::sort(array, cppsort::case_insensitive_less);
        CHECK( array == expected );
    }

    SECTION( "explicit global locale" )
    {
        std::locale locale;
        cppsort::sort(array, cppsort::case_insensitive_less(locale));
        CHECK( array == expected );
    }

    SECTION( "raw less customization point" )
    {
        sub::foo lhs, rhs;

        auto res1 = cppsort::case_insensitive_less(lhs, rhs);
        CHECK( res1 == sub::res_t::without_locale );

        auto res2 = cppsort::case_insensitive_less(std::locale())(lhs, rhs);
        CHECK( res2 == sub::res_t::with_locale );
    }

    SECTION( "refined less customization point" )
    {
        sub::foo lhs, rhs;

        auto res1 = cppsort::refined<sub::foo>(cppsort::case_insensitive_less)(lhs, rhs);
        CHECK( res1 == sub::res_t::without_locale );

        auto res2 = cppsort::refined<sub::foo>(cppsort::case_insensitive_less)(std::locale())(lhs, rhs);
        CHECK( res2 == sub::res_t::with_locale );
    }
}

