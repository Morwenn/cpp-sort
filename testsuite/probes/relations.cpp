/*
 * Copyright (c) 2016-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>

TEST_CASE( "relations between measures of presortedness", "[probe]" )
{
    constexpr std::vector<int>::difference_type size = 100;

    std::vector<int> sequence;
    sequence.reserve(size);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(sequence), size, 0);

    // The computer science literature lists a number of
    // relations between the results of different measures
    // of presortedness on a same sequence; the following
    // tests check that these relations are respected in
    // the library

    auto exc    = cppsort::probe::exc(sequence);
    auto ham    = cppsort::probe::ham(sequence);
    auto inv    = cppsort::probe::inv(sequence);
    auto max    = cppsort::probe::max(sequence);
    auto mono   = cppsort::probe::mono(sequence);
    auto par    = cppsort::probe::par(sequence);
    auto rem    = cppsort::probe::rem(sequence);
    auto runs   = cppsort::probe::runs(sequence);

    // Measures of Presortedness and Optimal Sorting Algorithms
    // by Heikki Mannila
    CHECK( exc <= inv );

    // A framework for adaptative sorting
    // by Ola Petersson and Alistair Moffat
    CHECK( runs <= rem + 1 );

    CHECK( exc + 1 <= ham );
    CHECK( ham <= 2 * exc );

    CHECK( max <= par );
    CHECK( par <= 2 * max );

    // A New Measure of Presortedness
    // by Vladimir Estivill-Castro and Derick Wood
    CHECK( par <= inv );
    CHECK( rem <= size * (1 - 1 / (par + 1)) );

    // Intuitive result: a descending run can be seen as several
    // ascending runs
    CHECK( mono <= runs );
}
