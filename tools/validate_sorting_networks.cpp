/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2018 Morwenn
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
#include <exception>
#include <iostream>
#include <iterator>
#include <utility>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/sort.h>

////////////////////////////////////////////////////////////
// Dedicated algorithm to find all permutations of an array
// of 0 and 1 and play an algorithm to each permutation,
// notably using a Gray code progression to find every
// permutation

template<typename RandomAccessIterator, typename Function>
auto for_each_gray_permutation(RandomAccessIterator first, RandomAccessIterator last,
                               Function func)
    -> void
{
    using unsigned_t = cppsort::detail::difference_type_t<RandomAccessIterator>;
    unsigned_t counter = 0u;

    unsigned_t end = 1 << (last - first);  // 2^n permutations
    for (unsigned_t nb_permutations = 0 ; nb_permutations != end ; nb_permutations += 2) {

        // Flip value for even counter
        func();
        counter ^= 1u;
        first[0u] = 1 - first[0u];

        // Flip value for odd counter
        func();
        auto y = counter & -counter;
        counter ^= (y << 1);
        auto to_flip = cppsort::detail::log2(y) + 1;
        first[to_flip] = 1 - first[to_flip];
    }
}

////////////////////////////////////////////////////////////
// Count the number of comparisons

template<typename T, std::size_t N>
auto validate_sorting_network()
    -> void
{
    if (N < 2) return;

    std::cout << "sorting network of size " << N << ": ";

    cppsort::sorting_network_sorter<N> sorter;
    std::array<T, N> collection;
    std::fill(std::begin(collection), std::end(collection), 0);

    try {
        for_each_gray_permutation(std::begin(collection), std::end(collection), [&] {
            // Copy then sort collection
            std::array<int, N> to_sort = collection;
            sorter(to_sort);

            // Check whether it is sorted
            if (not std::is_sorted(std::begin(to_sort), std::end(to_sort))) {
                // Log the faulty array
                std::cout << "\n  failed to sort the following input:\n  ";
                for (const auto& value: collection) {
                    std::cout << value << ' ';
                }
                std::cout << "\n  got the following output:\n  ";
                for (const auto& value: to_sort) {
                    std::cout << value << ' ';
                }
                std::cout << '\n';

                // Abort for this size
                throw std::exception();
            }
        });
    } catch (const std::exception&) {
        // Don't do anything else, go on with the next size
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
    using indices = std::make_index_sequence<23>;

    validate_sorting_networks<int>(indices{});
}
