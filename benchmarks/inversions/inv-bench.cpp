/*
 * Copyright (c) 2020-2023 Morwenn
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
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorters.h>
#include "../benchmarking-tools/cpu_cycles.h"
#include "../benchmarking-tools/distributions.h"
#include "../benchmarking-tools/filesystem.h"

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
    { "heap_sort",                      cppsort::heap_sort                                  },
    { "drop_merge_adapter(heap_sort)",  cppsort::drop_merge_adapter<cppsort::heap_sorter>{} },
    { "split_adapter(heap_sort)",       cppsort::split_adapter<cppsort::heap_sorter>{}      },
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
            auto distribution = dist::inv(factor);

            std::vector<std::uint64_t> cycles;

            auto total_start = clock_type::now();
            auto total_end = clock_type::now();
            while (total_end - total_start < max_run_time && cycles.size() < max_runs_per_size) {
                collection_t collection;
                distribution(std::back_inserter(collection), size);
                auto do_sort = cpu_cycles<sort_f>(sort.second);
                auto nb_cycles = do_sort(collection);
                assert(std::is_sorted(std::begin(collection), std::end(collection)));
                cycles.push_back(double(nb_cycles.value()) / size + 0.5);
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
