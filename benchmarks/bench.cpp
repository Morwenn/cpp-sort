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

#ifdef _WIN32
    #include <intrin.h>
    #define rdtsc __rdtsc
#else
    #ifdef __i586__
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

// Type of data to sort during the benchmark
using value_t = int;
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
        { "shuffled",               shuffled()              },
        { "shuffled_16_values",     shuffled_16_values()    },
        { "all_equal",              all_equal()             },
        { "ascending",              ascending()             },
        { "descending",             descending()            },
        { "pipe_organ",             pipe_organ()            },
        { "push_front",             push_front()            },
        { "push_middle",            push_middle()           },
        { "ascending_sawtooth",     ascending_sawtooth()    },
        { "descending_sawtooth",    descending_sawtooth()   },
        { "alternating",            alternating()           },
        { "alternating_16_values",  alternating_16_values() }
    };

    std::pair<std::string, sort_f> sorts[] = {
        { "heap_sort",      cppsort::heap_sort      },
        { "pdq_sort",       cppsort::pdq_sort       },
        { "quick_sort",     cppsort::quick_sort     },
        { "spread_sort",    cppsort::spread_sort    },
        { "std_sort",       cppsort::std_sort       },
        { "verge_sort",     cppsort::verge_sort     }
    };

    std::size_t sizes[] = { 1'000'000 };

    // Poor seed, yet enough for our benchmarks
    std::uint_fast32_t seed = std::time(nullptr);

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

                std::cerr << size << ' ' << distribution.first << ' ' << sort.first
                          << ' ' << cycles[cycles.size() / 2] << '\n';
                std::cout << size << ' ' << distribution.first << ' ' << sort.first
                          << ' ' << cycles[cycles.size() / 2] << '\n';
            }
        }
    }
}
