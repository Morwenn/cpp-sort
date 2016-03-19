#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <cpp-sort/sorters.h>
#include "distributions.h"

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

template<template<typename...> class Collection, typename T>
using distr_f = void (*)(std::back_insert_iterator<Collection<T>>, std::size_t);

template<template<typename...> class Collection, typename T>
using sort_f = void (*)(Collection<T>&);

int main()
{
    using namespace std::chrono_literals;

    std::pair<std::string, distr_f<std::vector, int>> distributions[] = {
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

    std::pair<std::string, sort_f<std::vector, int>> sorts[] = {
        { "heap_sort",      cppsort::heap_sort      },
        { "pdq_sort",       cppsort::pdq_sort       },
        { "quick_sort",     cppsort::quick_sort     },
        { "spread_sort",    cppsort::spread_sort    },
        { "std_sort",       cppsort::std_sort       },
        { "verge_sort",     cppsort::verge_sort     }
    };

    std::size_t sizes[] = { 1'000'000 };

    for (auto& distribution: distributions)
    {
        for (auto& sort: sorts)
        {
            for (auto size: sizes)
            {
                std::vector<std::uint64_t> cycles;

                auto total_start = std::chrono::high_resolution_clock::now();
                auto total_end = std::chrono::high_resolution_clock::now();
                while (std::chrono::duration_cast<std::chrono::seconds>(total_end - total_start) < 10s)
                {
                    std::vector<int> collection;
                    distribution.second(std::back_inserter(collection), size);
                    std::uint64_t start = rdtsc();
                    sort.second(collection);
                    std::uint64_t end = rdtsc();
                    assert(std::is_sorted(std::begin(collection), std::end(collection)));
                    cycles.push_back(double(end - start) / size + 0.5);
                    total_end = std::chrono::high_resolution_clock::now();
                }

                std::sort(std::begin(cycles), std::end(cycles));

                std::cerr << size << ' ' << distribution.first << ' ' << sort.first << '\n';
                std::cout << size << ' ' << distribution.first << ' ' << sort.first << ' ';
                for (std::uint64_t cycle: cycles)
                {
                    std::cout << cycle << ' ';
                }
                std::cout << '\n';
            }
        }
    }
}
