/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */

/*
    pdqsort.h - Pattern-defeating quicksort.

    Copyright (c) 2015 Orson Peters

    This software is provided 'as-is', without any express or implied warranty. In no event will the
    authors be held liable for any damages arising from the use of this software.

    Permission is granted to anyone to use this software for any purpose, including commercial
    applications, and to alter it and redistribute it freely, subject to the following restrictions:

    1. The origin of this software must not be misrepresented; you must not claim that you wrote the
       original software. If you use this software in a product, an acknowledgment in the product
       documentation would be appreciated but is not required.

    2. Altered source versions must be plainly marked as such, and must not be misrepresented as
       being the original software.

    3. This notice may not be removed or altered from any source distribution.
*/
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <iterator>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sorters.h>
#include "distributions.h"
#include "rdtsc.h"

// Type of data to sort during the benchmark
using value_t = double;
// Type of collection to sort
using collection_t = std::vector<value_t>;

// Handy function pointer aliases
using distr_f = void (*)(std::back_insert_iterator<collection_t>, std::size_t);
using sort_f = void (*)(collection_t&);

int main()
{
    using namespace std::chrono_literals;

    // Always use a steady clock
    using clock_type = std::conditional_t<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
    >;

    std::pair<std::string, distr_f> distributions[] = {
        { "shuffled",               dist::shuffled()                },
        { "shuffled_16_values",     dist::shuffled_16_values()      },
        { "all_equal",              dist::all_equal()               },
        { "ascending",              dist::ascending()               },
        { "descending",             dist::descending()              },
        { "pipe_organ",             dist::pipe_organ()              },
        { "push_front",             dist::push_front()              },
        { "push_middle",            dist::push_middle()             },
        { "ascending_sawtooth",     dist::ascending_sawtooth()      },
        { "descending_sawtooth",    dist::descending_sawtooth()     },
        { "alternating",            dist::alternating()             },
        { "alternating_16_values",  dist::alternating_16_values()   },
    };

    std::pair<std::string, sort_f> sorts[] = {
        { "heap_sort",      cppsort::heap_sort      },
        { "pdq_sort",       cppsort::pdq_sort       },
        { "quick_sort",     cppsort::quick_sort     },
        { "spread_sort",    cppsort::spread_sort    },
        { "std_sort",       cppsort::std_sort       },
        { "verge_sort",     cppsort::verge_sort     },
    };

    std::size_t sizes[] = { 1'000'000 };

    // Poor seed, yet enough for our benchmarks
    std::uint_fast32_t seed = std::time(nullptr);
    std::cerr << "SEED: " << seed << '\n';

    for (auto& distribution: distributions) {
        for (auto& sort: sorts) {
            // Seed the distribution manually to ensure that all algorithms
            // sort the same collections when there is randomness
            distributions_prng.seed(seed);

            for (auto size: sizes) {
                std::vector<std::uint64_t> cycles;

                auto total_start = clock_type::now();
                auto total_end = clock_type::now();
                while (std::chrono::duration_cast<std::chrono::seconds>(total_end - total_start) < 5s) {
                    collection_t collection;
                    distribution.second(std::back_inserter(collection), size);
                    std::uint64_t start = rdtsc();
                    sort.second(collection);
                    std::uint64_t end = rdtsc();
                    assert(std::is_sorted(std::begin(collection), std::end(collection)));
                    cycles.push_back(double(end - start) / size + 0.5);
                    total_end = clock_type::now();
                }

                std::sort(std::begin(cycles), std::end(cycles));

                std::cerr << size << ", " << distribution.first << ", " << sort.first
                          << ", " << cycles[cycles.size() / 2] << '\n';
                std::cout << size << ", " << distribution.first << ", " << sort.first
                          << ", " << cycles[cycles.size() / 2] << '\n';
            }
        }
    }
}
