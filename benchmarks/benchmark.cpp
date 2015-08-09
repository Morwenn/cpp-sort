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
#include <fstream>
#include <iterator>
#include <numeric>
#include <random>
#include <type_traits>
#include <utility>
#include <cpp-sort/sort.h>
#include "pdqsort.h"
#include "timsort.h"

////////////////////////////////////////////////////////////
// Sorting functions

template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto std_sort(RandomAccessIterable& iterable, Compare compare={})
    -> void
{
    std::sort(std::begin(iterable), std::end(iterable), compare);
}

template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto pdq_sort(RandomAccessIterable& iterable, Compare compare={})
    -> void
{
    pdqsort(std::begin(iterable), std::end(iterable), compare);
}

template<
    typename RandomAccessIterable,
    typename Compare = std::less<>
>
auto tim_sort(RandomAccessIterable& iterable, Compare&& compare={})
    -> void
{
    gfx::timsort(std::begin(iterable), std::end(iterable), compare);
}

////////////////////////////////////////////////////////////
// Distributions

struct shuffled
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        // Pseudo-random numbers generator
        thread_local std::mt19937_64 engine(std::time(nullptr));

        std::iota(begin, end, 0);
        std::shuffle(begin, end, engine);
    }

    static constexpr const char* output = "shuffled.txt";
};

struct all_equal
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        std::fill(begin, end, 0);
    }

    static constexpr const char* output = "all_equal.txt";
};

struct ascending
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        std::iota(begin, end, 0);
    }

    static constexpr const char* output = "ascending.txt";
};

struct descending
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        std::iota(begin, end, 0);
        std::reverse(begin, end);
    }

    static constexpr const char* output = "descending.txt";
};

struct pipe_organ
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        std::size_t size = std::distance(begin, end);
        std::size_t count = 0u;
        for (std::size_t i = 0 ; i < size / 2u ; ++i)
        {
            begin[count++] = i;
        }
        for (std::size_t i = size / 2 ; i < size ; ++i)
        {
            begin[count++] = size - i;
        }
    }

    static constexpr const char* output = "pipe_organ.txt";
};

struct push_front
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        if (std::distance(begin, end) > 0)
        {
            std::iota(begin, end, 1);
            *std::prev(end) = 0;
        }
    }

    static constexpr const char* output = "push_front.txt";
};

struct push_middle
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end)
        -> void
    {
        std::size_t size = std::distance(begin, end);
        if (size > 0)
        {
            std::size_t count = 0u;
            for (std::size_t i = 0u ; i < size ; ++i)
            {
                if (i != size / 2u)
                {
                    begin[count++] = i;
                }
            }
            begin[count] = size / 2;
        }
    }

    static constexpr const char* output = "push_middle.txt";
};

////////////////////////////////////////////////////////////
// Timing functions

template<
    typename T,
    std::size_t N,
    typename SortFunction,
    typename DistributionFunction
>
auto time_it(SortFunction sort, DistributionFunction dist, std::size_t times)
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
        sort(array, std::less<>{});
    }
    auto end = clock_type::now();

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
    // Compute results for the different sorting algorithms
    std::array<std::chrono::milliseconds, sizeof...(Ind)> results[] = {
        { time_it<T, Ind>(&cppsort::sort<T, Ind, std::less<>>, Distribution{}, times)... },
        { time_it<T, Ind>(&std_sort<std::array<T, Ind>, std::less<>>, Distribution{}, times)... },
        { time_it<T, Ind>(&tim_sort<std::array<T, Ind>, std::less<>>, Distribution{}, times)... },
        { time_it<T, Ind>(&pdq_sort<std::array<T, Ind>, std::less<>>, Distribution{}, times)... }
    };

    // Output the results to their respective files
    std::ofstream output(Distribution::output);
    for (auto&& sort_times: results)
    {
        for (auto&& time: sort_times)
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
    time_distributions<int, 40u,
        shuffled,
        all_equal,
        ascending,
        descending,
        pipe_organ,
        push_front,
        push_middle
    >(1'000'000u);
}
