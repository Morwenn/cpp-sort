/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <string>
#include <catch.hpp>
#include <cpp-sort/comparators/case_insensitive_less.h>
#include <cpp-sort/sort.h>

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
}

