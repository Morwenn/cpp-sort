/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <algorithm>
#include <array>
#include <iterator>
#include <catch.hpp>
#include <cpp-sort/sort.h>

TEST_CASE( "sort arrays of size 4", "[small_array_adapter][sort]" )
{
    SECTION( "sort C arrays" )
    {
        // Create a sorted array
        int array[] = { 1, 2, 3, 4 };

        // Try to sort every permutation
        do
        {
            int to_sort[] = { 0, 0, 0, 0 };
            std::copy(std::begin(array), std::end(array), std::begin(to_sort));
            cppsort::sort(to_sort);
            CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort)) );
        } while (std::next_permutation(std::begin(array), std::end(array)));
    }

    SECTION( "sort std::array" )
    {
        // Create a sorted array
        std::array<int, 4u> array = { 1, 2, 3, 4 };

        // Try to sort every permutation
        do
        {
            auto to_sort = array;
            cppsort::sort(to_sort);
            CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort)) );
        } while (std::next_permutation(std::begin(array), std::end(array)));
    }
}
