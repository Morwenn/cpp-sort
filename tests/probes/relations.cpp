/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <iterator>
#include <vector>
#include <catch2/catch_test_macros.hpp>
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

    auto block  = cppsort::probe::block(sequence);
    auto dis    = cppsort::probe::dis(sequence);
    auto enc    = cppsort::probe::enc(sequence);
    auto exc    = cppsort::probe::exc(sequence);
    auto ham    = cppsort::probe::ham(sequence);
    auto inv    = cppsort::probe::inv(sequence);
    auto max    = cppsort::probe::max(sequence);
    auto mono   = cppsort::probe::mono(sequence);
    auto osc    = cppsort::probe::osc(sequence);
    auto rem    = cppsort::probe::rem(sequence);
    auto runs   = cppsort::probe::runs(sequence);
    auto sus    = cppsort::probe::sus(sequence);

    // Measures of Presortedness and Optimal Sorting Algorithms
    // by Heikki Mannila
    CHECK( exc <= inv );

    // Splitsort - an adaptive sorting algorithm
    // by Christos Levcopoulos and Ola Petersson
    CHECK( rem <= inv );

    // A framework for adaptive sorting
    // by Ola Petersson and Alistair Moffat
    CHECK( runs <= rem + 1 );

    CHECK( exc + 1 <= ham );
    CHECK( ham <= 2 * exc );

    CHECK( max <= dis );
    CHECK( dis <= 2 * max );

    // A New Measure of Presortedness
    // by Vladimir Estivill-Castro and Derick Wood
    CHECK( dis <= inv );
    CHECK( rem <= size * (1 - 1 / (dis + 1)) );
    CHECK( inv <= size * dis / 2 );

    // Practical Adaptive Sorting
    // by Vladimir Estivill-Castro and Derick Wood
    CHECK( rem <= 2 * exc );

    // Encroaching lists as a measure of presortedness
    // by Steven S. Skiena
    CHECK( enc <= runs );
    CHECK( (2 * std::sqrt(enc) + 1) <= inv );
    CHECK( enc <= std::min(rem + 1, size - rem) );
    CHECK( 2 * enc <= exc );

    // Sorting Shuffled Monotone Sequences
    // by Christos Levcopoulos and Ola Petersson
    CHECK( sus <= runs );
    CHECK( sus <= max );
    CHECK( enc <= sus );

    // Heapsort - Adapted for Presorted Files
    // by Christos Levcopoulos and Ola Petersson
    CHECK( osc <= 4 * inv );
    CHECK( osc <= 2 * size * runs + size );
    CHECK( osc <= size * dis );

    // Sublinear Merging and Natural Mergesort
    // by Svante Carlsson, Christos Levcopoulos and Ola Petersson
    CHECK( block <= 3 * rem );

    // Computing and ranking measures of presortedness
    // by Jingsen Chen
    CHECK( enc <= dis + 1 );

    // Intuitive result: a descending run can be seen as several
    // ascending runs
    CHECK( mono <= runs );
}
