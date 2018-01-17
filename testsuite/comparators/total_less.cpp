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
#include <cmath>
#include <limits>
#include <catch.hpp>
#include <cpp-sort/comparators/total_less.h>
#include <cpp-sort/sort.h>

TEST_CASE( "IEEE 754 totalOrder implementation" )
{
    static constexpr double nan = std::numeric_limits<double>::quiet_NaN();
    static constexpr double inf = std::numeric_limits<double>::infinity();

    double array[] = { +1.0, +inf, -1.0, -nan, +0.0, -inf, +nan, -0.0 };
    cppsort::sort(array, cppsort::total_less);

    // Check for IEEE 754 totalOrder,
    // ignore quiet vs. signaling NaNs
    CHECK( std::isnan(array[0]) );
    CHECK( std::signbit(array[0]) );
    CHECK( std::isinf(array[1]) );
    CHECK( std::signbit(array[1]) );
    CHECK( array[2] == -1.0 );
    CHECK( array[3] == 0.0 );
    CHECK( std::signbit(array[3]) );
    CHECK( array[4] == 0.0 );
    CHECK( not std::signbit(array[4]) );
    CHECK( array[5] == +1.0 );
    CHECK( std::isinf(array[6]) );
    CHECK( not std::signbit(array[6]) );
    CHECK( std::isnan(array[7]) );
    CHECK( not std::signbit(array[7]) );
}
