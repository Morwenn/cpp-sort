/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cassert>
#include <forward_list>
#include <functional>
#include <vector>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorters.h>

int main()
{
    struct wrapper { int value; };

    std::forward_list<wrapper> li = { {5}, {8}, {3}, {2}, {9} };
    std::vector<wrapper> vec = { {5}, {8}, {3}, {2}, {9} };

    // When used, this sorter will use a pattern-defeating quicksort
    // to sort random-access collections, and a mergesort otherwise
    cppsort::hybrid_adapter<
        cppsort::pdq_sorter,
        cppsort::merge_sorter
    > sorter;

    // Sort li and vec in reverse order using their value member
    sorter(li, std::greater<>{}, &wrapper::value);
    sorter(vec, std::greater<>{}, &wrapper::value);

    assert(std::equal(
        li.begin(), li.end(),
        vec.begin(), vec.end(),
        [](const auto& lhs, const auto& rhs) { return lhs.value == rhs.value; }
    ));
}
