/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <array>
#include <locale>
#include <string>
#include <catch2/catch.hpp>
#include <cpp-sort/comparators/case_insensitive_less.h>
#include <cpp-sort/refined.h>
#include <cpp-sort/sorters/heap_sorter.h>

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

    inline auto case_insensitive_less(const foo&, const foo&, const std::locale&)
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
        cppsort::heap_sort(array, cppsort::case_insensitive_less);
        CHECK( array == expected );
    }

    SECTION( "explicit global locale" )
    {
        std::locale locale;
        cppsort::heap_sort(array, cppsort::case_insensitive_less(locale));
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

