/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/probes.h>
#include "../distributions.h"

TEST_CASE( "relations between measures of presortedness", "[probe]" )
{
    std::vector<int> sequence; sequence.reserve(100);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(sequence), 100, 0);

    // The computer science literature lists a number of
    // relations between the results of different measures
    // of presortedness on a same sequence; the following
    // tests check that these relations are respected in
    // the library

    auto exc    = cppsort::probe::exc(sequence);
    auto ham    = cppsort::probe::ham(sequence);
    auto inv    = cppsort::probe::inv(sequence);
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

    // A New Measure of Presortedness
    // by Vladimir Estivill-Castro and Derick Wood
    CHECK( par <= inv );
    CHECK( rem <= sequence.size() * (1 - 1 / (par + 1)) );
}
