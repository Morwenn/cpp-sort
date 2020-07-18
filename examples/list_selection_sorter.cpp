/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
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
