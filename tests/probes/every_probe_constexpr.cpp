/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/probes.h>

template<typename MOP>
constexpr auto test_constexpr_mop()
    -> auto
{
    MOP mop;
    int collection[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    return mop(collection);
}

TEMPLATE_TEST_CASE( "test every probe in a constexpr context", "[probe][constexpr]",
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::runs) )
{
    constexpr int presortedness = test_constexpr_mop<TestType>();
    CHECK( presortedness == 0 );
}
