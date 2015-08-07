/*
 * Copyright (C) 2015 Morwenn
 *
 * array_sort is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * array_sort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program. If not,
 * see <http://www.gnu.org/licenses/>.
 */
#include <algorithm>
#include <array>
#include <chrono>
#include <ctime>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <utility>
#include <cpp-sort/sort.h>

template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto std_sort(RandomAccessIterable& iterable, Compare&& compare={})
    -> void
{
    std::sort(std::begin(iterable), std::end(iterable), std::forward<Compare>(compare));
}

template<
    typename T,
    std::size_t N,
    typename SortFunction
>
auto time_it(SortFunction sort, std::size_t times)
    -> std::chrono::milliseconds
{
    // Choose the best clock type (always steady)
    using clock_type = std::conditional_t<
        std::chrono::high_resolution_clock::is_steady,
        std::chrono::high_resolution_clock,
        std::chrono::steady_clock
    >;

    // Random numbers utilities
    thread_local std::mt19937_64 engine(std::time(nullptr));

    // Generate N shuffled arrays
    std::vector<std::array<T, N>> arrays(times);
    for (auto&& array: arrays)
    {
        std::iota(std::begin(array), std::end(array), 0);
        std::shuffle(std::begin(array), std::end(array), engine);
    }

    // Time while sorting the arrays
    auto start = clock_type::now();
    for (auto&& array: arrays)
    {
        sort(array, std::less<>{});
    }
    auto end = clock_type::now();

    // Return the time it took to sort the arrays
    return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
}

template<typename T, std::size_t... Ind>
auto time_them(std::size_t size, std::index_sequence<Ind...>)
    -> std::array<
            std::array<
                std::chrono::milliseconds,
                sizeof...(Ind)
            >,
            2u
        >
{
    // Results for cppsort::sort
    std::array<std::chrono::milliseconds, sizeof...(Ind)> first = {
        { time_it<T, Ind>(&cppsort::sort<T, Ind, std::less<>>, size)... }
    };

    // Results for std::sort
    std::array<std::chrono::milliseconds, sizeof...(Ind)> second = {
        { time_it<T, Ind>(&std_sort<std::array<T, Ind>, std::less<>>, size)... }
    };

    return { first, second };
}

int main()
{
    using indices = std::make_index_sequence<40u>;
    auto sorts_times = time_them<int>(1000000u, indices{});

    for (auto&& sort_times: sorts_times)
    {
        for (auto&& time: sort_times)
        {
            std::cout << time.count() << ' ';
        }
        std::cout << '\n';
    }
}
