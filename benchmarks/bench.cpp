#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <iterator>
#include <random>
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
using distr_f = void (*)(typename Collection<T>::iterator, typename Collection<T>::iterator);

template<template<typename...> class Collection, typename T>
using sort_f = void (*)(Collection<T>&);

int main()
{
    auto seed = std::time(0);
    std::mt19937_64 el;

    std::pair<std::string, distr_f<std::vector, int>> distributions[] = {
        { "shuffled_int",               shuffled()                },
        { "shuffled_16_values_int",     shuffled_16_values()      },
        { "all_equal_int",              all_equal()               },
        { "ascending_int",              ascending()               },
        { "descending_int",             descending()              },
        { "pipe_organ_int",             pipe_organ()              },
        { "push_front_int",             push_front()              },
        { "push_middle_int",            push_middle()             },
        { "ascending_sawtooth_int",     ascending_sawtooth()      },
        { "descending_sawtooth_int",    descending_sawtooth()     },
        { "alternating_int",            alternating()             },
        { "alternating_16_values_int",  alternating_16_values()   }
    };

    std::pair<std::string, sort_f<std::vector, int>> sorts[] = {
        { "heapsort",   cppsort::heap_sorter()      },
        { "introsort",  cppsort::std_sorter()       },
        { "pdqsort",    cppsort::pdq_sorter()       },
        { "vergesort",  cppsort::verge_sorter()     },
        { "timsort",    cppsort::tim_sorter()       },
        { "spreadsort", cppsort::spread_sorter()    }
    };

    int sizes[] = { 1'000'000 };

    for (auto& distribution: distributions) {
        for (auto& sort: sorts) {
            el.seed(seed);

            for (auto size: sizes) {
                std::chrono::time_point<std::chrono::high_resolution_clock> total_start, total_end;
                std::vector<std::uint64_t> cycles;

                total_start = std::chrono::high_resolution_clock::now();
                total_end = std::chrono::high_resolution_clock::now();
                while (std::chrono::duration_cast<std::chrono::milliseconds>(total_end - total_start).count() < 10000) {
                    std::vector<int> vec(size);
                    distribution.second(std::begin(vec), std::end(vec));
                    std::uint64_t start = rdtsc();
                    sort.second(vec);
                    std::uint64_t end = rdtsc();
                    assert(std::is_sorted(std::begin(vec), std::end(vec)));
                    cycles.push_back(double(end - start) / size + 0.5);
                    total_end = std::chrono::high_resolution_clock::now();
                }

                std::sort(cycles.begin(), cycles.end());

                std::cerr << size << ' ' << distribution.first << ' ' << sort.first << '\n';
                std::cout << size << ' ' << distribution.first << ' ' << sort.first << ' ';
                for (std::uint64_t cycle: cycles) std::cout << cycle << ' ';
                std::cout << '\n';
            }
        }
    }
}
