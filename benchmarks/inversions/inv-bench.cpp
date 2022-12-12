/*
 * Copyright (c) 2020-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <cpp-sort/sorters.h>
#include "../benchmarking-tools/distributions.h"
#include "../benchmarking-tools/filesystem.h"
#include "../benchmarking-tools/rdtsc.h"

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
    { "drop_merge_sort",    cppsort::drop_merge_sort    },
    { "pdq_sort",           cppsort::pdq_sort           },
    { "split_sort",         cppsort::split_sort         },
};

// Size of the collections to sort
constexpr std::size_t size = 1'000'000;

// Maximum time to let the benchmark run for a given size before giving up
auto max_run_time = 5s;
// Maximum number of benchmark runs per size
std::size_t max_runs_per_size = 25;


////////////////////////////////////////////////////////////
// Benchmark code proper

int main(int argc, char* argv[])
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

    int sort_number = 0;
    for (auto& sort: sorts) {
        // Create a file to store the results
        auto sort_number_str = std::to_string(sort_number);
        auto output_filename =
            std::string(3 - sort_number_str.size(), '0') +
            std::move(sort_number_str) +
            '-' + safe_file_name(sort.first) + ".csv";
        std::string output_path = output_directory + '/' + output_filename;
        std::ofstream output_file(output_path);
        output_file << sort.first << '\n';
        std::cout << sort.first << '\n';

        // Seed the distribution manually to ensure that all algorithms
        // sort the same collections when there is randomness
        distributions_prng.seed(seed);

        for (int idx = 0; idx <= 100; ++idx) {
            double factor = 0.01 * idx;
            auto distribution = dist::inversions(factor);

            std::vector<std::uint64_t> cycles;

            auto total_start = clock_type::now();
            auto total_end = clock_type::now();
            while (std::chrono::duration_cast<std::chrono::seconds>(total_end - total_start) < max_run_time &&
                   cycles.size() < max_runs_per_size) {
                collection_t collection;
                distribution(std::back_inserter(collection), size);
                std::uint64_t start = rdtsc();
                sort.second(collection);
                std::uint64_t end = rdtsc();
                assert(std::is_sorted(std::begin(collection), std::end(collection)));
                cycles.push_back(double(end - start) / size + 0.5);
                total_end = clock_type::now();
            }

            // Compute and display stats & numbers
            output_file << idx << ",";
            std::cout << idx << ",";
            auto it = cycles.begin();
            output_file << *it;
            std::cout << *it;
            while (++it != cycles.end()) {
                output_file << "," << *it;
                std::cout << "," << *it;
            }
            output_file << '\n';
            std::cout << std::endl;
        }
        ++sort_number;
    }
}
