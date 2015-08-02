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
#include <string>
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
    typename SortFunction1,
    typename SortFunction2
>
auto time_compare(SortFunction1 sort1, SortFunction2 sort2, std::size_t times)
    -> std::array<std::chrono::milliseconds, 2u>
{
    // Random numbers generator
    thread_local std::mt19937_64 engine(std::time(nullptr));

    // Generate shuffled array, the same for both algorithms
    std::array<T, N> array;
    std::iota(std::begin(array), std::end(array), 0);
    std::shuffle(std::begin(array), std::end(array), engine);

    // Time first algorithm
    auto start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0 ; i < times ; ++i)
    {
        auto unsorted = array;
        sort1(unsorted, std::less<>{});
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // Time second algorithm
    start = std::chrono::high_resolution_clock::now();
    for (std::size_t i = 0 ; i < times ; ++i)
    {
        auto unsorted = array;
        sort2(unsorted, std::less<>{});
    }
    end = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    return { duration1, duration2 };
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
    // Benchmark the sorts std::sort
    std::array<std::array<std::chrono::milliseconds, 2u>, sizeof...(Ind)> results = {
        time_compare<T, Ind>(
            &cppsort::sort<T, Ind, std::less<>>,
            &std_sort<std::array<T, Ind>, std::less<>>,
            size
        )...
    };

    // Results for cppsort::sort
    std::array<std::chrono::milliseconds, sizeof...(Ind)> first = {
        std::get<Ind>(results)[0u]...
    };

    // Results for std::sort
    std::array<std::chrono::milliseconds, sizeof...(Ind)> second = {
        std::get<Ind>(results)[1u]...
    };

    return { first, second };
}

int main()
{
    using indices = std::make_index_sequence<64u>;
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
