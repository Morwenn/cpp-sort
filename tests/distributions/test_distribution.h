/*
 * Copyright (c) 2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_TEST_DISTRIBUTION_H_
#define CPPSORT_TESTSUITE_TEST_DISTRIBUTION_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <algorithm>
#include <forward_list>
#include <iterator>
#include <list>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>

namespace helpers
{
    template<typename Collection, typename Size, typename Distribution, typename... Args>
    auto init_collection(Collection& collection, Size size,
                         Distribution distribution, Args&&... args)
        -> void
    {
        distribution(std::back_inserter(collection), size, std::forward<Args>(args)...);
    }

    template<typename T, typename Size, typename Distribution, typename... Args>
    auto init_collection(std::vector<T>& collection, Size size,
                         Distribution distribution, Args&&... args)
        -> void
    {
        collection.reserve(size);
        distribution(std::back_inserter(collection), size, std::forward<Args>(args)...);
    }

    template<typename T, typename Size, typename Distribution, typename... Args>
    auto init_collection(std::forward_list<T>& collection, Size size,
                         Distribution distribution, Args&&... args)
        -> void
    {
        distribution(std::front_inserter(collection), size, std::forward<Args>(args)...);
        collection.reverse();
    }

    template<
        typename Sorter,
        typename Collection,
        typename Size,
        typename Distribution,
        typename... Args
    >
    auto test_distribution(Collection& collection, Size size,
                           Distribution distribution, Args&&... args)
        -> void
    {
        init_collection(collection, size, distribution, std::forward<Args>(args)...);

        auto copy = collection;

        Sorter sorter;
        sorter(collection);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );

        // Check that the result is a permutation of the original collection
        cppsort::merge_sort(copy);
        CHECK( collection == copy );
    }
}

#endif // CPPSORT_TESTSUITE_TEST_DISTRIBUTION_H_