/*
 * Copyright (c) 2015-2023 Morwenn
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
#include "../benchmarking-tools/cpu_cycles.h"
#include "../benchmarking-tools/distributions.h"

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
    typename Dist
>
auto time_it(Sorter sorter, Dist distribution)
    -> std::uint64_t
{
    static_assert(N > 0, "this benchmark does not support zero-sized arrays");

    // Seed the distribution manually to ensure that all algorithms
    // sort the same collections when there is randomness
    distributions_prng.seed(seed);

    std::vector<std::uint64_t> cycles;

    // Generate and sort arrays of size N thanks to distribution
    auto total_start = clock_type::now();
    auto total_end = clock_type::now();
    while (total_end - total_start < max_run_time && cycles.size() < max_runs_per_size) {
        std::array<T, N> arr;
        distribution(arr.begin(), N);
        auto do_sort = cpu_cycles<Sorter>(sorter);
        auto nb_cycles = do_sort(arr);
        assert(std::is_sorted(arr.begin(), arr.end()));
        cycles.push_back(double(nb_cycles.value()) / N);
        total_end = clock_type::now();
    }

    // Return the median number of cycles per element
    auto cycles_median = cycles.begin() + cycles.size() / 2;
    std::nth_element(cycles.begin(), cycles_median, cycles.end());
    return *cycles_median;
}

template<
    typename T,
    typename Dist,
    std::size_t... Ind
>
auto time_distribution(std::index_sequence<Ind...>)
    -> void
{
    using low_comparisons_sorter = cppsort::small_array_adapter<
        cppsort::low_comparisons_sorter
    >;
    using low_moves_sorter = cppsort::small_array_adapter<
        cppsort::low_moves_sorter
    >;
    using merge_exchange_network_sorter = cppsort::small_array_adapter<
        cppsort::merge_exchange_network_sorter
    >;
    using sorting_network_sorter = cppsort::small_array_adapter<
        cppsort::sorting_network_sorter
    >;

    // Compute results for the different sorting algorithms
    std::pair<const char*, std::array<std::uint64_t, sizeof...(Ind)>> results[] = {
        { "insertion_sorter",               { time_it<T, Ind + 1>(cppsort::insertion_sort,          Dist{})... } },
        { "selection_sorter",               { time_it<T, Ind + 1>(cppsort::selection_sort,          Dist{})... } },
        { "low_comparisons_sorter",         { time_it<T, Ind + 1>(low_comparisons_sorter{},         Dist{})... } },
        { "low_moves_sorter",               { time_it<T, Ind + 1>(low_moves_sorter{},               Dist{})... } },
        { "merge_exchange_network_sorter",  { time_it<T, Ind + 1>(merge_exchange_network_sorter{},  Dist{})... } },
        { "sorting_network_sorter",         { time_it<T, Ind + 1>(sorting_network_sorter{},         Dist{})... } },
    };

    // Output the results to their respective files
    std::ofstream output(Dist::output);
    for (auto&& sort_result: results) {
        output << std::get<0>(sort_result) << ',';
        for (auto&& nb_cycles: std::get<1>(sort_result)) {
            output << nb_cycles << ',';
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
    using indices = std::make_index_sequence<N - 1>;

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
