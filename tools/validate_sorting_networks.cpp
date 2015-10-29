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
#include <cstddef>
#include <iostream>
#include <iterator>
#include <utility>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/sort.h>

////////////////////////////////////////////////////////////
// Count the number of comparisons

template<typename T, std::size_t N>
auto validate_sorting_network()
    -> void
{
    std::cout << "sorting network of size " << N << ": ";

    cppsort::sorting_network_sorter<N> sorter;
    std::array<T, N> collection;

    for (auto it = std::begin(collection) ; it != std::end(collection) ; ++it)
    {
        // Progressively fill collection with 1s
        std::fill(std::begin(collection), it, 0);
        std::fill(it, std::end(collection), 1);

        // For each possible permutation of collection
        do
        {
            // Copy then sort collection
            std::array<int, N> to_sort = collection;
            cppsort::sort(to_sort, sorter);

            // Check whether it is sorted
            if (not std::is_sorted(std::begin(to_sort), std::end(to_sort)))
            {
                // Log the faulty array
                std::cout << "\n  failed to sort the following input:\n  ";
                for (const auto& value: collection)
                {
                    std::cout << value << ' ';
                }
                std::cout << '\n';

                // Abort for this size
                return;
            }
        } while (std::next_permutation(std::begin(collection), std::end(collection)));
    }

    std::cout << "ok\n";
}

template<typename T, std::size_t... Indices>
auto validate_sorting_networks(std::index_sequence<Indices...>)
    -> void
{
    // Variadic dispatch only works with expressions
    int dummy[] = {
        (validate_sorting_network<T, Indices>(), 0)...
    };
    (void) dummy;
}


////////////////////////////////////////////////////////////
// Main

int main()
{
    // The zero-one principle states that a sorting network is
    // valid if it can sort all the permutations of 0 and 1 for
    // a given size of array. This program uses this principle
    // to validate the sorting networks in O(2^n) instead of
    // the slower O(n!).

    // Sizes of the sorting networks to validate
    using indices = std::make_index_sequence<25>;

    validate_sorting_networks<int>(indices{});
}
