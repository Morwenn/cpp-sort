/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2020 Morwenn
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
#include <catch2/catch.hpp>
#include <cpp-sort/comparators/natural_less.h>
#include <cpp-sort/sorters/heap_sorter.h>

TEST_CASE( "string natural sort with natural_less" )
{
    std::array<std::string, 7> array = {
        "Yay",
        "Yay 32 lol",
        "Yuy 32 lol",
        "Yay 045",
        "Yay 01245 huhuhu",
        "Yay 45",
        "Yay 1234"
    };
    cppsort::heap_sort(array, cppsort::natural_less);

    std::array<std::string, 7> expected = {
        "Yay",
        "Yay 32 lol",
        "Yay 45",
        "Yay 045",
        "Yay 1234",
        "Yay 01245 huhuhu",
        "Yuy 32 lol"
    };
    CHECK( array == expected );
}

