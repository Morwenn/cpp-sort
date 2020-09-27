/*
 * Copyright (c) 2018-2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <cpp-sort/sorters/smooth_sorter.h>

int main()
{
    std::array<int, 5> arr = { 5, 8, 3, 2, 9 };
    cppsort::smooth_sort(arr);
    assert(std::is_sorted(arr.begin(), arr.end()));

    // prints 2 3 5 8 9
    for (int val: arr) {
        std::cout << val << ' ';
    }
}
