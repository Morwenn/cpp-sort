/*
 * Copyright (c) 2016-2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/as_function.h>

namespace example
{
    template<typename T, typename Allocator=std::allocator<T>>
    struct list:
        std::list<T, Allocator>
    {
        // Yes, we cheat and simply inherit from std::list
        template<typename... Args>
        list(Args&&... args):
            std::list<T, Allocator>(std::forward<Args>(args)...)
        {}
    };

    template<typename T, typename Allocator, typename Compare=std::less<>>
    auto sort(cppsort::selection_sorter, list<T, Allocator>& collection,
              Compare compare={})
        -> void
    {
        // Dedicated selection sort algorithm for example::list

        auto&& comp = cppsort::utility::as_function(compare);

        auto it = std::begin(collection);
        auto last = std::end(collection);
        while (it != last) {
            auto min_it = std::min_element(it, last, comp);
            if (min_it == it) {
                ++it;
            } else {
                collection.splice(it, collection, min_it);
            }
        }
    }
}

#include <cassert>
#include <ctime>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>
#include <cpp-sort/adapters/container_aware_adapter.h>

int main()
{
    // Initialize list with random integers
    std::vector<int> vec(40, 0);
    std::iota(std::begin(vec), std::end(vec), 0);
    std::mt19937_64 engine(std::time(nullptr));
    std::shuffle(std::begin(vec), std::end(vec), engine);
    example::list<int> li(std::begin(vec), std::end(vec));

    // Sort with container_aware_adapter
    cppsort::container_aware_adapter<
        cppsort::selection_sorter
    > sorter;
    sorter(li);

    assert(std::is_sorted(std::begin(li), std::end(li)));
}
