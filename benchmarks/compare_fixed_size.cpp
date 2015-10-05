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
#include <cassert>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <cpp-sort/adapters/counting_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/sorters.h>

using namespace cppsort;

////////////////////////////////////////////////////////////
// Readable sorter names

template<typename Sorter>
struct sorter_name;

template<>
struct sorter_name<insertion_sorter>
{
    static constexpr const char* value = "insertion_sorter";
};

template<>
struct sorter_name<selection_sorter>
{
    static constexpr const char* value = "selection_sorter";
};

template<typename... Args>
struct sorter_name<small_array_adapter<Args...>>
{
    static constexpr const char* value = "small_array_sorter";
};

////////////////////////////////////////////////////////////
// Counting functions

template<std::size_t N, typename Sorter>
auto count_comparison()
    -> void
{
    // Fill an array of size N
    std::array<int, N> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // Count comparisons made by a sorter
    cppsort::counting_adapter<Sorter> sorter;

    // Total number of comparisons
    std::size_t count = 0;

    // For each possible permutation of collection
    do
    {
        auto to_sort = collection;
        // Sort collection, get the number of comparisons made
        count += sorter(to_sort);
        // Double counts as sorter tests
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort)));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));

    // Display the result
    std::cout << std::left
              << std::setw(20)  << sorter_name<Sorter>::value
              << std::setw(6)   << count
              << '\n';
}

template<std::size_t N, typename... Sorters>
auto compare_fixed_size()
    -> void
{
    std::cout << "array of size " << N << '\n';

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (count_comparison<N, Sorters>(), 0)...
    };
    (void) dummy;
}

////////////////////////////////////////////////////////////
// Main

int main()
{
    // Size of the arrays to sort
    static constexpr std::size_t size = 7;

    compare_fixed_size<size,
        insertion_sorter,
        selection_sorter,
        small_array_adapter<void>
    >();
}
