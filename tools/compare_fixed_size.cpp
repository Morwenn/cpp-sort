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
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
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

template<std::size_t N>
struct sorter_name<low_comparisons_sorter<N>>
{
    static constexpr const char* value = "low_comparisons_sorter";
};

template<std::size_t N>
struct sorter_name<low_moves_sorter<N>>
{
    static constexpr const char* value = "low_moves_sorter";
};

template<>
struct sorter_name<selection_sorter>
{
    static constexpr const char* value = "selection_sorter";
};

template<std::size_t N>
struct sorter_name<sorting_network_sorter<N>>
{
    static constexpr const char* value = "sorting_network_sorter";
};

template<template<std::size_t> class Sorter, typename Indices>
struct sorter_name<small_array_adapter<Sorter, Indices>>
{
    static constexpr const char* value = sorter_name<Sorter<0u>>::value;
};

////////////////////////////////////////////////////////////
// Count the number of comparisons

template<std::size_t N, typename Sorter, std::size_t... Ind>
auto count_comparisons(std::index_sequence<Ind...>)
    -> void
{
    cppsort::counting_adapter<Sorter, unsigned long long> sorter;

    // Fill an array of size N
    std::array<int, N> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // Total number of comparisons
    unsigned long long count = 0;

    // For each possible permutation of collection
    do
    {
        std::array<int, N> to_sort = { std::get<Ind>(collection)... };
        // Sort collection, get the number of comparisons made
        count += sorter(to_sort);

        // Double counts as sorter tests
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort)));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));

    // Display the result
    std::cout << std::left
              << std::setw(25)  << sorter_name<Sorter>::value
              << std::setw(6)   << count
              << '\n';
}

template<std::size_t N, typename... Sorters>
auto compare_comparisons()
    -> void
{
    std::cout << "\ncomparisons needed to sort an array of size " << N << '\n';

    using indices = std::make_index_sequence<N>;

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (count_comparisons<N, Sorters>(indices{}), 0)...
    };
    (void) dummy;
}

////////////////////////////////////////////////////////////
// Count the number of moves

template<typename T>
struct move_counter
{
    //
    // This structure is used to count the number of moves
    // performed by a sorting algorithm and additionally
    // checks that no read from moved-from values happen
    // during the sort
    //

    // Constructors

    move_counter() = delete;
    move_counter(const move_counter&) = delete;

    move_counter(const T& value):
        moves(0),
        can_read(true),
        value(value)
    {}

    move_counter(move_counter&& other):
        moves(other.moves + 1),
        can_read(true),
        value(std::move(other.value))
    {
        if (not std::exchange(other.can_read, false))
        {
            std::cout << "illegal read from a moved-from value\n";
            assert(false);
        }
        other.moves = 0;
    }

    // Assignment operators

    move_counter& operator=(const move_counter&) = delete;

    auto operator=(move_counter&& other)
        -> move_counter&
    {
        if (&other != this)
        {
            moves += other.moves + 1;
            if (not std::exchange(other.can_read, false))
            {
                std::cout << "illegal read from a moved-from value\n";
                assert(false);
            }
            can_read = true;
            value = std::move(other.value);
            other.moves = 0;
        }
        return *this;
    }

    // Number of moves since its creation
    std::size_t moves;
    // Whether the value can be read
    bool can_read;
    // Actual value
    T value;
};

template<typename T>
auto operator<(const move_counter<T>& lhs, const move_counter<T>& rhs)
    -> bool
{
    return lhs.value < rhs.value;
}

template<std::size_t N, typename Sorter, std::size_t... Ind>
auto count_moves(std::index_sequence<Ind...>)
    -> void
{
    Sorter sorter;

    // Fill an array of size N
    std::array<int, N> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // Total number of moves
    std::size_t count = 0;

    // For each possible permutation of collection
    do
    {
        std::array<move_counter<int>, N> to_sort = { std::get<Ind>(collection)... };

        // Sort collection, get the number of loves performed
        sorter(to_sort);
        for (auto& val: to_sort)
        {
            count += val.moves;
        }

        // Double counts as sorter tests
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort)));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));

    // Display the result
    std::cout << std::left
              << std::setw(25)  << sorter_name<Sorter>::value
              << std::setw(6)   << count
              << '\n';
}

template<std::size_t N, typename... Sorters>
auto compare_moves()
    -> void
{
    std::cout << "\nmoves needed to sort and array of size " << N << '\n';

    using indices = std::make_index_sequence<N>;

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (count_moves<N, Sorters>(indices{}), 0)...
    };
    (void) dummy;
}

////////////////////////////////////////////////////////////
// Measure the number of cycles

#ifdef _WIN32
    #include <intrin.h>
    #define rdtsc __rdtsc
#else
    #ifdef __i386__
        static __inline__ unsigned long long rdtsc() {
            unsigned long long int x;
            __asm__ volatile(".byte 0x0f, 0x31" : "=A" (x));
            return x;
        }
    #elif defined(__x86_64__)
        static __inline__ unsigned long long rdtsc(){
            unsigned hi, lo;
            __asm__ __volatile__("rdtsc" : "=a"(lo), "=d"(hi));
            return ((unsigned long long) lo) | (((unsigned long long) hi) << 32);
        }
    #else
        #error no rdtsc implementation
    #endif
#endif

template<std::size_t N, typename Sorter, std::size_t... Ind>
auto count_cycles(std::index_sequence<Ind...>)
    -> void
{
    Sorter sorter;

    // Fill an array of size N
    std::array<int, N> collection;
    std::iota(std::begin(collection), std::end(collection), 0);

    // Total number of cycles
    std::size_t count = 0;

    // For each possible permutation of collection
    do
    {
        std::array<int, N> to_sort = { std::get<Ind>(collection)... };
        // Sort collection, get the number of cycles
        auto start = rdtsc();
        sorter(to_sort);
        auto end = rdtsc();

        count += (end - start);

        // Double counts as sorter tests
        assert(std::is_sorted(std::begin(to_sort), std::end(to_sort)));
    } while (std::next_permutation(std::begin(collection), std::end(collection)));

    // Display the result
    std::cout << std::left
              << std::setw(25)  << sorter_name<Sorter>::value
              << std::setw(6)   << count
              << '\n';
}

template<std::size_t N, typename... Sorters>
auto compare_time()
    -> void
{
    std::cout << "\ncycles needed to sort an array of size " << N << '\n';

    using indices = std::make_index_sequence<N>;

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (count_cycles<N, Sorters>(indices{}), 0)...
    };
    (void) dummy;
}

////////////////////////////////////////////////////////////
// Main

int main()
{
    // Size of the arrays to sort
    static constexpr std::size_t size = 13;

    using network_sorter = small_array_adapter<
        sorting_network_sorter
    >;

    using comparisons_sorter = small_array_adapter<
        low_comparisons_sorter
    >;

    using moves_sorter = small_array_adapter<
        low_moves_sorter
    >;

    compare_comparisons<size,
        insertion_sorter,
        selection_sorter,
        moves_sorter,
        comparisons_sorter,
        network_sorter
    >();

    compare_moves<size,
        insertion_sorter,
        selection_sorter,
        moves_sorter,
        comparisons_sorter,
        network_sorter
    >();

    compare_time<size,
        insertion_sorter,
        selection_sorter,
        moves_sorter,
        comparisons_sorter,
        network_sorter
    >();
}
