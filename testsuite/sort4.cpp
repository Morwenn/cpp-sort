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

TEST_CASE( "sort arrays of size 4", "[sort]" )
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
