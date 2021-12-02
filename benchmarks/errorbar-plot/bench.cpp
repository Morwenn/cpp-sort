/*
 * Copyright (c) 2020-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <ratio>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <cpp-sort/sorters.h>
#include "../benchmarking-tools/distributions.h"
#include "../benchmarking-tools/filesystem.h"
#include "../benchmarking-tools/statistics.h"

using namespace std::chrono_literals;

////////////////////////////////////////////////////////////
// Benchmark configuration variables

// Type of data to sort during the benchmark
using value_t = int;
// Type of collection to sort
using collection_t = std::vector<value_t>;

// Sorting algorithms to benchmark
using sort_f = void (*)(collection_t&);
std::pair<std::string, sort_f> sorts[] = {
    { "heap_sort",      cppsort::heap_sort      },
    { "poplar_sort",    cppsort::poplar_sort    },
    { "smooth_sort",    cppsort::smooth_sort    }
};

// Distribution to benchmark against
auto distribution = dist::shuffled{};

// Sizes of the collections to sort
std::uint64_t size_min = 1u << 1;
std::uint64_t size_max = 1u << 22;

// Maximum time to let the benchmark run for a given size before giving up
auto max_run_time = 60s;
// Maximum number of benchmark runs per size
std::size_t max_runs_per_size = 25;


////////////////////////////////////////////////////////////
// Benchmark code proper

int main(int argc, char** argv)
{
    // Choose the output directory
    std::string output_directory = ".";
    if (argc > 1) {
        output_directory = argv[1];
    }

    // Always use a steady clock
    using clock_type = std::conditional_t<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
    >;

    // Poor seed, yet enough for our benchmarks
    std::uint_fast32_t seed = std::time(nullptr);
    std::cout << "SEED: " << seed << '\n';

    for (auto& sort: sorts) {
        // Create a file to store the results
        std::string output_filename = output_directory + '/' + safe_file_name(sort.first) + ".csv";
        std::ofstream output_file(output_filename);
        output_file << sort.first << '\n';
        std::cout << sort.first << '\n';

        // Seed the distribution manually to ensure that all algorithms
        // sort the same collections when there is randomness
        distributions_prng.seed(seed);

        // Sort the collection as long as needed
        std::uint64_t pow_of_2 = 0;  // For logs
        for (auto size = size_min ; size <= size_max ; size <<= 1) {
            std::vector<double> times;

            auto total_start = clock_type::now();
            auto total_end = clock_type::now();
            while (total_end - total_start < max_run_time && times.size() < max_runs_per_size) {
                collection_t collection;
                distribution(std::back_inserter(collection), size);
                auto start = clock_type::now();
                sort.second(collection);
                auto end = clock_type::now();
                assert(std::is_sorted(std::begin(collection), std::end(collection)));
                times.push_back(std::chrono::duration<double, std::milli>(end - start).count());
                total_end = clock_type::now();
            }

            // Compute and display stats & numbers
            double avg = average(times);

            std::ostringstream ss;
            ss << ++pow_of_2 << ", "
               << size << ", "
               << avg << ", "
               << standard_deviation(times, avg) << '\n';
            output_file << ss.str();
            std::cout << ss.str();

            // Abort if the allocated time was merely enough to benchmark a single run
            if (times.size() < 2) break;
        }
    }
}
