/*
 * Copyright (c) 2015-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <array>
#include <atomic>
#include <cstddef>
#include <exception>
#include <functional>
#include <iostream>
#include <iterator>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/detail/bitops.h>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/sort.h>

#define CPPSORT_LIKELY(x) __builtin_expect((x), 1)
#define CPPSORT_UNLIKELY(x) __builtin_expect((x), 0)

////////////////////////////////////////////////////////////
// Dedicated algorithm to find all permutations of an array
// of 0 and 1 and play an algorithm to each permutation,
// notably using a Gray code progression to find every
// permutation

template<typename RandomAccessIterator, typename T, std::size_t N, typename Function>
auto for_each_gray_permutation_inner(RandomAccessIterator first, RandomAccessIterator last,
                                     std::array<T, N>& collection, Function func)
    -> bool
{
    using unsigned_t = std::make_unsigned_t<
        cppsort::detail::difference_type_t<RandomAccessIterator>
    >;
    unsigned_t counter = 0u;

    unsigned_t end = 1 << (last - first);  // 2^n permutations
    for (unsigned_t nb_permutations = 0 ; nb_permutations != end ; nb_permutations += 2) {

        // Flip value for even counter
        if (CPPSORT_UNLIKELY(not func(collection))) {
            return false;
        }
        counter ^= 1u;
        first[0u] = 1 - first[0u];

        // Flip value for odd counter
        if (CPPSORT_UNLIKELY(not func(collection))) {
            return false;
        }
        auto y = counter & -counter;
        counter ^= (y << 1);
        auto to_flip = cppsort::detail::log2(y) + 1;
        first[to_flip] = 1 - first[to_flip];
    }
    return true;
}

template<typename T, std::size_t N, typename Function>
auto for_each_gray_permutation(std::array<T, N>& collection, Function func)
    -> void
{
    using iterator = typename std::array<T, N>::iterator;
    using unsigned_t = std::make_unsigned_t<
        cppsort::detail::difference_type_t<iterator>
    >;

    unsigned_t size = collection.size();
    // Compute the "header" of bits before launching parallel jobs
    unsigned_t header_size = cppsort::detail::log2(std::thread::hardware_concurrency());

    if (size <= 20 || size < header_size) {
        // The problem size is small enough or we can't gain
        // enough from parallel jobs, go full sequential
        for_each_gray_permutation_inner(collection.begin(), collection.end(),
                                        collection, std::move(func));
    } else {
        // The goal of the following algorithm is to use a many threads as possible, etc.
        std::vector<std::thread> jobs;

        auto first = collection.begin();
        for_each_gray_permutation_inner(first, first + header_size, collection, [&](std::array<T, N> arr) {
            jobs.emplace_back(for_each_gray_permutation_inner<iterator, T, N, Function&>,
                              arr.begin() + header_size, arr.end(),
                              std::ref(arr), std::ref(func));
            return true;
        });

        // Join the jobs before exiting
        for (auto& job: jobs) {
            job.join();
        }
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
    std::fill(collection.begin(), collection.end(), 0);

    std::atomic<bool> error_occurred(false);
    bool expected = false;
    for_each_gray_permutation(collection, [&](const std::array<T, N>& arr) {
        // Copy then sort collection
        std::array<int, N> to_sort = arr;
        sorter(to_sort);

        // Check whether it is sorted
        if (not std::is_sorted(std::begin(to_sort), std::end(to_sort))) {
            if (error_occurred.compare_exchange_strong(expected, true)) {
                // Log the faulty array
                std::cout << "\n  failed to sort the following input:\n  ";
                for (const auto& value: arr) {
                    std::cout << value << ' ';
                }
                std::cout << "\n  got the following output:\n  ";
                for (const auto& value: to_sort) {
                    std::cout << value << ' ';
                }
                std::cout << '\n';
            }
            // Abort for this size
            return false;
        }
        return true;
    });
    std::cout << "ok\n";
}

template<typename T, std::size_t... Indices>
auto validate_sorting_networks(std::index_sequence<Indices...>)
    -> void
{
    // Variadic dispatch only works with expressions
    int dummy[] = {
        (validate_sorting_network<T, Indices + 1>(), 0)...
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

    // Best time to check all sorting networks to date: 536s

    // Sizes of the sorting networks to validate
    using indices = std::make_index_sequence<26>;

    validate_sorting_networks<int>(indices{});
}
