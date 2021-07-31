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

template<typename Sorter>
void test(const char* name)
{
    const int size = 5000;

    std::vector<std::string> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), size, dist::as_long_string{});

    auto copy = collection;
    cppsort::quick_sort(std::begin(copy), std::end(copy));

    auto sorter = Sorter{};
    sorter(collection);
    auto copy2 = collection;

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
    } else {
        std::cout << "is it the same as the one sorted with quicksort? ";
        std::cout << (collection == copy) << std::endl;
        std::cout << "were some elements altered? ";
        cppsort::quick_sort(std::begin(collection), std::end(collection));
        std::cout << (collection != copy) << std::endl;
    }

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
        << "rem: " << cppsort::probe::rem(copy2) << std::endl
        << "runs: " << cppsort::probe::runs(copy2) << std::endl
        << "sus: " << cppsort::probe::sus(copy2) << std::endl
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
