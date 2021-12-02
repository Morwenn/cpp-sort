/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters.h>
#include "../benchmarking-tools/distributions.h"
#include "../benchmarking-tools/rdtsc.h"

using namespace std::chrono_literals;

// Choose the best clock type (always steady)
using clock_type = std::conditional_t<
    std::chrono::high_resolution_clock::is_steady,
    std::chrono::high_resolution_clock,
    std::chrono::steady_clock
>;

// Maximum time to let the benchmark run for a given size before giving up
auto max_run_time = 5s;
// Maximum number of benchmark runs per size
std::size_t max_runs_per_size = 300;

// Poor seed, yet enough for our benchmarks
std::uint_fast32_t seed = std::time(nullptr);

////////////////////////////////////////////////////////////
// Timing functions

template<
    typename T,
    std::size_t N,
    typename Sorter,
    typename DistributionFunction
>
auto time_it(Sorter sorter, DistributionFunction distribution)
    -> double
{
    // Seed the distribution manually to ensure that all algorithms
    // sort the same collections when there is randomness
    distributions_prng.seed(seed);

    std::vector<std::uint64_t> cycles;

    // Generate and sort arrays of size N thanks to distribution
    auto total_start = clock_type::now();
    auto total_end = clock_type::now();
    while (std::chrono::duration_cast<std::chrono::seconds>(total_end - total_start) < max_run_time &&
           cycles.size() < max_runs_per_size) {
        std::array<T, N> arr;
        distribution(arr.begin(), N);
        std::uint64_t start = rdtsc();
        sorter(arr);
        std::uint64_t end = rdtsc();
        assert(std::is_sorted(arr.begin(), arr.end()));
        cycles.push_back(end - start);
        total_end = clock_type::now();
    }

    // Return the average number of cycles it took to sort the arrays
    std::uint64_t avg = 0;
    for (auto value: cycles) {
        avg += value;
    }
    return avg / double(cycles.size());
}

template<
    typename T,
    typename Distribution,
    std::size_t... Ind
>
auto time_distribution(std::index_sequence<Ind...>)
    -> void
{
    using sorting_network_sorter = cppsort::small_array_adapter<
        cppsort::sorting_network_sorter
    >;

    using low_comparisons_sorter = cppsort::small_array_adapter<
        cppsort::low_comparisons_sorter
    >;

    using low_moves_sorter = cppsort::small_array_adapter<
        cppsort::low_moves_sorter
    >;

    // Compute results for the different sorting algorithms
    std::pair<const char*, std::array<double, sizeof...(Ind)>> results[] = {
        { "insertion_sorter",       { time_it<T, Ind>(cppsort::insertion_sort,  Distribution{})... } },
        { "selection_sorter",       { time_it<T, Ind>(cppsort::selection_sort,  Distribution{})... } },
        { "low_moves_sorter",       { time_it<T, Ind>(low_moves_sorter{},       Distribution{})... } },
        { "low_comparisons_sorter", { time_it<T, Ind>(low_comparisons_sorter{}, Distribution{})... } },
        { "sorting_network_sorter", { time_it<T, Ind>(sorting_network_sorter{}, Distribution{})... } },
    };

    // Output the results to their respective files
    std::ofstream output(Distribution::output);
    for (auto&& sort_result: results) {
        output << std::get<0>(sort_result) << ' ';
        for (auto&& nb_cycles: std::get<1>(sort_result)) {
            output << nb_cycles << ' ';
        }
        output << '\n';
    }
}

template<
    typename T,
    std::size_t N,
    typename... Distributions
>
auto time_distributions()
    -> void
{
    using indices = std::make_index_sequence<N>;

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (time_distribution<T, Distributions>(indices{}), 0)...
    };
    (void) dummy;
}

int main()
{
    std::cout << "SEED: " << seed << '\n';

    time_distributions<int, 13u,
        dist::shuffled,
        dist::all_equal,
        dist::ascending,
        dist::descending,
        dist::pipe_organ,
        dist::push_front,
        dist::push_middle
    >();
}
