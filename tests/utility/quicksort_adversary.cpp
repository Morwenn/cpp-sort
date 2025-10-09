/*
 * Copyright (c) 2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/detail/iter_sort3.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/metrics/comparisons.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/quicksort_adversary.h>

TEMPLATE_TEST_CASE( "test quicksort-based sorters with quicksort_adversary", "[utility][quicksort_adversary]",
                    cppsort::pdq_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::std_sorter )
{
    cppsort::metrics::comparisons<TestType> sorter;
    auto comps = cppsort::utility::quicksort_adversary(sorter, 1000);
    CHECK( comps < 100'000 ); // Guesstimate between n and n²
}

namespace
{
    // Median-of-three quicksort
    template<typename Iterator, typename Compare>
    auto median_of_3_quicksort(Iterator first, Iterator last, Compare comp)
        -> void
    {
        auto size = last - first;
        if (size < 2) return;

        auto middle = first + size / 2;
        auto pivot_pos = cppsort::detail::iter_sort3(
            first, middle, last - 1,
            comp, cppsort::utility::identity{}
        );

        iter_swap(pivot_pos, last - 1);
        auto middle1 = std::partition(
            first, last - 1,
            [&](int& value) { return comp(value, *(last - 1)); }
        );

        iter_swap(middle1, last - 1);
        auto middle2 = std::partition(
            std::next(middle1), last,
            [&](int& value) { return not comp(*middle1, value); }
        );

        median_of_3_quicksort(first, middle1, comp);
        median_of_3_quicksort(middle2, last, comp);
    }
}


TEST_CASE( "quicksort adversary over a simple quicksort",
           "[utility][quicksort_adversary]" )
{
    auto do_sort = [](std::vector<int>& vec, auto comp) {
        return median_of_3_quicksort(vec.begin(), vec.end(), comp);
    };
    auto sorter = cppsort::metrics::comparisons(do_sort);
    auto comps = cppsort::utility::quicksort_adversary(sorter, 100);
    CHECK( comps > 5000 ); // Guesstimate over n²
}
