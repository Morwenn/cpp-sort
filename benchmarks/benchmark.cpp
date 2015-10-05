/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <array>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters.h>
#include "distributions.h"

////////////////////////////////////////////////////////////
// Timing functions

template<
    typename T,
    std::size_t N,
    typename Sorter,
    typename DistributionFunction
>
auto time_it(Sorter sorter, DistributionFunction dist, std::size_t times)
    -> std::chrono::milliseconds
{
    // Choose the best clock type (always steady)
    using clock_type = std::conditional_t<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
    >;

    // Generate times arrays of size N thanks to dist
    std::vector<std::array<T, N>> arrays(times);
    for (auto&& array: arrays)
    {
        dist(std::begin(array), std::end(array));
    }

    // Time while sorting the arrays
    auto start = clock_type::now();
    for (auto&& array: arrays)
    {
        cppsort::sort(array, sorter);
    }
    auto end = clock_type::now();

    // Double benchmark as unit test
    for (auto&& array: arrays)
    {
        assert(std::is_sorted(std::begin(array), std::end(array)));
    }

    // Return the time it took to sort the arrays
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

template<
    typename T,
    typename Distribution,
    std::size_t... Ind
>
auto time_distribution(std::size_t times, std::index_sequence<Ind...>)
    -> void
{
    using fixed_sorter = cppsort::small_array_adapter<
        cppsort::pdq_sorter
    >;

    // Compute results for the different sorting algorithms
    std::pair<const char*, std::array<std::chrono::milliseconds, sizeof...(Ind)>> results[] = {
        { "fixed_sorter",       { time_it<T, Ind>(fixed_sorter{},               Distribution{}, times)... } },
        { "std_sorter",         { time_it<T, Ind>(cppsort::std_sorter{},        Distribution{}, times)... } },
        { "tim_sorter",         { time_it<T, Ind>(cppsort::tim_sorter{},        Distribution{}, times)... } },
        { "pdq_sorter",         { time_it<T, Ind>(cppsort::pdq_sorter{},        Distribution{}, times)... } },
        { "insertion_sorter",   { time_it<T, Ind>(cppsort::insertion_sorter{},  Distribution{}, times)... } }
    };

    // Output the results to their respective files
    std::ofstream output(Distribution::output);
    for (auto&& sort_result: results)
    {
        output << std::get<0>(sort_result) << ' ';
        for (auto&& time: std::get<1>(sort_result))
        {
            output << time.count() << ' ';
        }
        output << '\n';
    }
}

template<
    typename T,
    std::size_t N,
    typename... Distributions
>
auto time_distributions(std::size_t times)
    -> void
{
    using indices = std::make_index_sequence<N>;

    // Variadic dispatch only works with expressions
    int dummy[] = {
        (time_distribution<T, Distributions>(times, indices{}), 0)...
    };
    (void) dummy;
}

int main()
{
    time_distributions<int, 15u,
        shuffled,
        all_equal,
        ascending,
        descending,
        pipe_organ,
        push_front,
        push_middle
    >(1'000'000u);
}
