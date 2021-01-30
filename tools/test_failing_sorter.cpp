/*
 * Copyright (c) 2019-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iostream>
#include <iterator>
#include <random>
#include <string>
#include <utility>
#include <vector>

#define CPPSORT_ENABLE_ASSERTIONS
//#define CPPSORT_ENABLE_AUDITS

#include <cpp-sort/adapters.h>
#include <cpp-sort/probes.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include "../benchmarks/benchmarking-tools/distributions.h"
#include "../testsuite/testing-tools/algorithm.h"
#include "../testsuite/testing-tools/wrapper.h"

struct shuffled_string:
    dist::base_distribution<shuffled_string>
{
    template<typename OutputIterator, typename T=long long int>
    auto operator()(OutputIterator out, long long int size, T start=T(0)) const
        -> void
    {
        // Pseudo-random number generator
        thread_local std::mt19937 engine(15321);

        std::vector<std::string> vec;
        vec.reserve(size);

        T end = start + size;
        for (auto i = start ; i < end ; ++i) {
            auto s = std::to_string(i);
            vec.push_back(std::string(50 - s.size(), '0') + std::move(s));
        }
        std::shuffle(std::begin(vec), std::end(vec), engine);
        std::move(std::begin(vec), std::end(vec), out);
    }
};

template<typename Sorter>
void test(const char* name)
{
    const int size = 412;

    std::vector<std::string> collection;
    auto distribution = shuffled_string{};
    distribution(std::back_inserter(collection), size);

    auto copy = collection;
    cppsort::quick_sort(std::begin(copy), std::end(copy));

    auto sorter = Sorter{};
    sorter(collection);

    // Collect basic data
    auto first_unsorted_it = std::is_sorted_until(std::begin(collection), std::end(collection));
    bool is_collection_sorted = first_unsorted_it == std::end(collection);

    // Display information
    std::cout << std::boolalpha << name << std::endl;
    std::cout << "is the collection sorted? ";
    std::cout << is_collection_sorted << std::endl;
    if (not is_collection_sorted) {
        std::cout << "position of the first unsorted element: "
                  << std::distance(std::begin(collection), first_unsorted_it)
                  << std::endl;
    }
    std::cout << "is it the same as the one sorted with std::sort? ";
    std::cout << (collection == copy) << std::endl;
    std::cout << "were some elements altered? ";
    auto copy2 = collection;
    cppsort::quick_sort(std::begin(collection), std::end(collection));
    std::cout << (collection != copy) << std::endl;

    // Measures of presortedness
    std::cout << '\n'
        << "dis: " << cppsort::probe::dis(copy2) << std::endl
        << "enc: " << cppsort::probe::enc(copy2) << std::endl
        << "exc: " << cppsort::probe::exc(copy2) << std::endl
        << "ham: " << cppsort::probe::ham(copy2) << std::endl
        << "inv: " << cppsort::probe::inv(copy2) << std::endl
        << "max: " << cppsort::probe::max(copy2) << std::endl
        << "mono: " << cppsort::probe::mono(copy2) << std::endl
        << "osc: " << cppsort::probe::osc(copy2) << std::endl
        << "par: " << cppsort::probe::par(copy2) << std::endl
        << "rem: " << cppsort::probe::rem(copy2) << std::endl
        << "runs: " << cppsort::probe::runs(copy2) << std::endl
        << '\n';

    if (size < 40) {
        std::cout << "unsorted collection:\n";
        for (const auto& elem: copy2) {
            std::cout << elem << ' ';
        }
        std::cout << "\n\n" << std::flush;
    }
}

int main()
{
    test<cppsort::poplar_sorter>("poplar_sort");
}
