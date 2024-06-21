/*
 * Copyright (c) 2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>
#include <cpp-sort/probes.h>
#include "../benchmarking-tools/distributions.h"

////////////////////////////////////////////////////////////
// Configuration variables

// Distribution
using dist_t = dist::runs;

// Measure of presortedness
auto mop = cppsort::probe::runs;

// Size of the collections to check
constexpr std::size_t size = 1'000'000;

////////////////////////////////////////////////////////////
// Generate data for distribution
//
// The raison d'être of this script is to be able to visualize
// the aspects of the distributions used to test whether some
// sorter adapt to given measures of presortedness: for example
// dist::inv, when given a percentage pct, should be able to
// create a random collection X such as:
//    prove::inv(X) = pct * probe::inv.max_for_size(|X|)
//
// Being able to visualize these results helps to check the
// soundness of the distributions.

int main()
{
    // Seed the distribution PRNG manually to ensure reproducibility
    std::uint_fast32_t seed = std::time(nullptr);
    distributions_prng.seed(seed);

    // Print metadata about the check
    std::cout << dist_t::name << ','
              << size << ','
              << mop.max_for_size(size) << ','
              << seed << std::endl;

    for (int idx = 0; idx <= 100; ++idx) {
        // Generate data distribution
        double factor = 0.01 * idx;
        auto distribution = dist_t(factor);

        // Compute presortedness
        std::vector<int> collection;
        collection.reserve(size);
        distribution(std::back_inserter(collection), size);
        auto presortedness = mop(collection);

        // Display results
        std::cout << idx << ',' << presortedness << std::endl;
    }
}
