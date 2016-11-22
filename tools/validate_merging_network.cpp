/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <cpp-sort/utility/iter_move.h>

////////////////////////////////////////////////////////////
// Helper function

template<typename Iterator>
auto iter_swap_if(Iterator lhs, Iterator rhs)
    -> void
{
    if (*rhs < *lhs) {
        using cppsort::utility::iter_swap;
        iter_swap(lhs, rhs);
    }
}

////////////////////////////////////////////////////////////
// Merging networks to test

template<std::size_t M, std::size_t N>
struct merging_network
{
    static_assert(
        N && false,
        "merging_network has no specialization for this size of M+N"
    );
};

template<>
struct merging_network<16, 12>
{
    template<typename T>
    auto operator()(std::array<T, 28>& arr) const
    {
        auto first = std::begin(arr);

        iter_swap_if(first + 0u, first + 16u);
        iter_swap_if(first + 8u, first + 24u);
        iter_swap_if(first + 8u, first + 16u);
        iter_swap_if(first + 4u, first + 20u);
        iter_swap_if(first + 12u, first + 20u);
        iter_swap_if(first + 4u, first + 8u);
        iter_swap_if(first + 12u, first + 16u);
        iter_swap_if(first + 20u, first + 24u);
        iter_swap_if(first + 2u, first + 18u);
        iter_swap_if(first + 10u, first + 26u);
        iter_swap_if(first + 10u, first + 18u);
        iter_swap_if(first + 6u, first + 22u);
        iter_swap_if(first + 14u, first + 22u);
        iter_swap_if(first + 6u, first + 10u);
        iter_swap_if(first + 14u, first + 18u);
        iter_swap_if(first + 22u, first + 26u);
        iter_swap_if(first + 2u, first + 4u);
        iter_swap_if(first + 6u, first + 8u);
        iter_swap_if(first + 10u, first + 12u);
        iter_swap_if(first + 14u, first + 16u);
        iter_swap_if(first + 18u, first + 20u);
        iter_swap_if(first + 22u, first + 24u);
        iter_swap_if(first + 1u, first + 17u);
        iter_swap_if(first + 9u, first + 25u);
        iter_swap_if(first + 9u, first + 17u);
        iter_swap_if(first + 5u, first + 21u);
        iter_swap_if(first + 13u, first + 21u);
        iter_swap_if(first + 5u, first + 9u);
        iter_swap_if(first + 13u, first + 17u);
        iter_swap_if(first + 21u, first + 25u);
        iter_swap_if(first + 3u, first + 19u);
        iter_swap_if(first + 11u, first + 27u);
        iter_swap_if(first + 11u, first + 19u);
        iter_swap_if(first + 7u, first + 23u);
        iter_swap_if(first + 15u, first + 23u);
        iter_swap_if(first + 7u, first + 11u);
        iter_swap_if(first + 15u, first + 19u);
        iter_swap_if(first + 23u, first + 27u);
        iter_swap_if(first + 3u, first + 5u);
        iter_swap_if(first + 7u, first + 9u);
        iter_swap_if(first + 11u, first + 13u);
        iter_swap_if(first + 15u, first + 17u);
        iter_swap_if(first + 19u, first + 21u);
        iter_swap_if(first + 23u, first + 25u);
        iter_swap_if(first + 1u, first + 2u);
        iter_swap_if(first + 3u, first + 4u);
        iter_swap_if(first + 5u, first + 6u);
        iter_swap_if(first + 7u, first + 8u);
        iter_swap_if(first + 9u, first + 10u);
        iter_swap_if(first + 11u, first + 12u);
        iter_swap_if(first + 13u, first + 14u);
        iter_swap_if(first + 15u, first + 16u);
        iter_swap_if(first + 17u, first + 18u);
        iter_swap_if(first + 19u, first + 20u);
        iter_swap_if(first + 21u, first + 22u);
        iter_swap_if(first + 23u, first + 24u);
        iter_swap_if(first + 25u, first + 26u);
    }
};

////////////////////////////////////////////////////////////
// Test merging network with zero-one principle

template<typename T, std::size_t M, std::size_t N>
auto validate_merging_network()
    -> void
{
    std::cout << "merging network of size " << M << '+' << N << ": ";

    merging_network<M, N> merger;
    std::array<T, M+N> collection;
    auto begin = std::begin(collection);
    auto end = std::end(collection);

    std::fill(begin, end, 1);

    // Loop on every combination of sorted M + N sub-collections
    for (std::size_t i = 0 ; i <= M ; ++i) {
        if (i != 0) {
            collection[i - 1] = 0;
        }
        std::fill(begin + M, end, 1);
        for (std::size_t j = M ; j <= M+N ; ++j) {
            if (j != M) {
                collection[j - 1] = 0;
            }

            // Copy then merge collection
            std::array<int, M+N> to_merge = collection;
            merger(to_merge);

            // Check whether it is sorted
            if (not std::is_sorted(std::begin(to_merge), std::end(to_merge))) {
                // Log the faulty array
                std::cout << "\n  failed to merge the following input:\n  ";
                for (const auto& value: collection) {
                    std::cout << value << ' ';
                }
                std::cout << "\n  got the following output:\n  ";
                for (const auto& value: to_merge) {
                    std::cout << value << ' ';
                }
                std::cout << '\n';

                // Abort for this size
                return;
            }
        }
    }

    std::cout << "ok\n";
}

////////////////////////////////////////////////////////////
// Main

int main()
{
    // The zero-one principle states that a sorting network is
    // valid if it can sort all the permutations of 0 and 1 for
    // a given size of array. This program uses this principle
    // to validate some merging networks (if the merged array is
    // sorted, the property holds).

    validate_merging_network<int, 16, 12>();
}
