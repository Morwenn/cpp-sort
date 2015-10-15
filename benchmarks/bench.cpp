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

template<typename T, template<typename...> class Collection>
Collection<T> shuffled_int(size_t size, std::mt19937_64& rng) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(i);
    std::shuffle(v.begin(), v.end(), rng);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> shuffled_16_values_int(size_t size, std::mt19937_64& rng) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(i % 16);
    std::shuffle(v.begin(), v.end(), rng);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> all_equal_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(0);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> ascending_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(i);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> descending_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = size - 1; i >= 0; --i) v.push_back(i);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> pipe_organ_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size/2; ++i) v.push_back(i);
    for (int i = size/2; i < size; ++i) v.push_back(size - i);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> push_front_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 1; i < size; ++i) v.push_back(i);
    v.push_back(0);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> push_middle_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) {
        if (i != size/2) v.push_back(i);
    }
    v.push_back(size/2);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> ascending_sawtooth_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    int limit = size / log2(size) * 1.1;
    for (int i = 0; i < size; ++i) v.push_back(i % limit);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> descending_sawtooth_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    int limit = size / log2(size) * 1.1;
    for (int i = size - 1; i >= 0; --i) v.push_back(i % limit);
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> alternating_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(i);
    for (int i = 0; i < size; i += 2) v[i] *= -1;
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}

template<typename T, template<typename...> class Collection>
Collection<T> alternating_16_values_int(size_t size, std::mt19937_64&) {
    std::vector<T> v; v.reserve(size);
    for (int i = 0; i < size; ++i) v.push_back(i % 16);
    for (int i = 0; i < size; i += 2) v[i] *= -1;
    return Collection<T>(
        std::make_move_iterator(std::begin(v)),
        std::make_move_iterator(std::end(v))
    );
}


int main()
{
    auto seed = std::time(0);
    std::mt19937_64 el;

    using distr_f    = std::vector<int>(*)(size_t, std::mt19937_64&);
    using sort_f     = void(*)(std::vector<int>&);

    std::pair<std::string, distr_f> distributions[] = {
        { "shuffled_int",               shuffled_int                },
        { "shuffled_16_values_int",     shuffled_16_values_int      },
        { "all_equal_int",              all_equal_int               },
        { "ascending_int",              ascending_int               },
        { "descending_int",             descending_int              },
        { "pipe_organ_int",             pipe_organ_int              },
        { "push_front_int",             push_front_int              },
        { "push_middle_int",            push_middle_int             },
        { "ascending_sawtooth_int",     ascending_sawtooth_int      },
        { "descending_sawtooth_int",    descending_sawtooth_int     },
        { "alternating_int",            alternating_int             },
        { "alternating_16_values_int",  alternating_16_values_int   }
    };

    std::pair<std::string, sort_f> sorts[] = {
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
                    std::vector<int> vec = distribution.second(size, el);
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
