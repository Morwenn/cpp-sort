/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Morwenn
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
#include <catch2/catch.hpp>
#include <cpp-sort/probes.h>
#include <testing-tools/distributions.h>
#include <testing-tools/functional_checks.h>

TEMPLATE_TEST_CASE( "every probe with comparison function altered by move", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto mop = probe{}(collection, move_check_compare{});
    CHECK( mop >= 0 );
}

TEMPLATE_TEST_CASE( "every probe with projection function altered by move", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::par),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs) )
{
    std::vector<int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    using probe = TestType;
    auto mop = probe{}(collection, move_check_projection{});
    CHECK( mop >= 0 );
}
