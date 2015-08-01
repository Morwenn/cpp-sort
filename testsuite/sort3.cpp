/*
 * Copyright (C) 2015 Morwenn
 *
 * array_sort is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * array_sort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#include <algorithm>
#include <array>
#include <iterator>
#include <catch.hpp>
#include <cpp-sort/sort.h>

TEST_CASE( "sort arrays of size 3", "[sort]" )
{
    SECTION( "sort C arrays" )
    {
        int sorted[] = { 1, 2, 3 };

        int arr0[] = { 1, 2, 3 };
        int arr1[] = { 1, 3, 2 };
        int arr2[] = { 2, 1, 3 };
        int arr3[] = { 2, 3, 1 };
        int arr4[] = { 3, 1, 2 };
        int arr5[] = { 3, 2, 1 };

        cppsort::sort(arr0);
        cppsort::sort(arr1);
        cppsort::sort(arr2);
        cppsort::sort(arr3);
        cppsort::sort(arr4);
        cppsort::sort(arr5);

        CHECK( std::equal(std::begin(arr0), std::end(arr0), std::begin(sorted), std::end(sorted)) );
        CHECK( std::equal(std::begin(arr1), std::end(arr1), std::begin(sorted), std::end(sorted)) );
        CHECK( std::equal(std::begin(arr2), std::end(arr2), std::begin(sorted), std::end(sorted)) );
        CHECK( std::equal(std::begin(arr3), std::end(arr3), std::begin(sorted), std::end(sorted)) );
        CHECK( std::equal(std::begin(arr4), std::end(arr4), std::begin(sorted), std::end(sorted)) );
        CHECK( std::equal(std::begin(arr5), std::end(arr5), std::begin(sorted), std::end(sorted)) );
    }

    SECTION( "sort std::array" )
    {
        std::array<int, 3u> sorted = { 1, 2, 3 };

        std::array<int, 3u> arr0 = { 1, 2, 3 };
        std::array<int, 3u> arr1 = { 1, 3, 2 };
        std::array<int, 3u> arr2 = { 2, 1, 3 };
        std::array<int, 3u> arr3 = { 2, 3, 1 };
        std::array<int, 3u> arr4 = { 3, 1, 2 };
        std::array<int, 3u> arr5 = { 3, 2, 1 };

        cppsort::sort(arr0);
        cppsort::sort(arr1);
        cppsort::sort(arr2);
        cppsort::sort(arr3);
        cppsort::sort(arr4);
        cppsort::sort(arr5);

        CHECK( arr0 == sorted );
        CHECK( arr1 == sorted );
        CHECK( arr2 == sorted );
        CHECK( arr3 == sorted );
        CHECK( arr4 == sorted );
        CHECK( arr5 == sorted );
    }
}
