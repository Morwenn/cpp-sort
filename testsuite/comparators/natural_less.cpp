/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
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

