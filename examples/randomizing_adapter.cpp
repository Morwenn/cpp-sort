/*
 * Copyright (c) 2022-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <random>
#include <type_traits>
#include <utility>
#include <cpp-sort/mstd/iterator.h>
#include <cpp-sort/mstd/ranges.h>
#include <cpp-sort/sorters/smooth_sorter.h>
#include <cpp-sort/utility/adapter_storage.h>

namespace mstd = cppsort::mstd;

template<
    mstd::random_access_iterator Iterator,
    mstd::sentinel_for<Iterator> Sentinel
>
auto my_shuffle(Iterator begin, Sentinel end)
    -> Iterator
{
    thread_local std::random_device device;
    thread_local std::minstd_rand engine(device());
    return std::ranges::shuffle(begin, end, engine);
}

template<typename Sorter>
struct randomizing_adapter:
    cppsort::utility::adapter_storage<Sorter>
{
    ////////////////////////////////////////////////////////////
    // Construction

    randomizing_adapter() = default;

    constexpr explicit randomizing_adapter(Sorter sorter):
        cppsort::utility::adapter_storage<Sorter>(std::move(sorter))
    {}

    ////////////////////////////////////////////////////////////
    // operator()

    template<
        typename Self,
        mstd::random_access_iterator Iterator,
        mstd::sentinel_for<Iterator> Sentinel,
        typename... Args
    >
    auto operator()(this Self&& self, Iterator begin, Sentinel end, Args&&... args)
        -> decltype(std::forward<Self>(self).get()(begin, begin, std::forward<Args>(args)...))
    {
        auto end_it = my_shuffle(begin, end);
        return std::forward<Self>(self).get()(begin, end, std::forward<Args>(args)...);
    }

    template<
        typename Self,
        mstd::random_access_range Range,
        typename... Args
    >
    auto operator()(this Self&& self, Range&& range, Args&&... args)
        -> decltype(std::forward<Self>(self).get()(std::forward<Range>(range), std::forward<Args>(args)...))
    {
        my_shuffle(mstd::begin(range), mstd::end(range));
        return std::forward<Self>(self).get()(std::forward<Range>(range), std::forward<Args>(args)...);
    }

    ////////////////////////////////////////////////////////////
    // Sorter traits

    using iterator_category = std::random_access_iterator_tag;
    using is_always_stable = std::false_type;
};

#include <iostream>
#include <vector>

int main()
{
    // Vector of pairs:
    // - First fields all contain the same value
    // - Second fields have unique values
    std::vector<std::pair<int, int>> vec;
    for (int i = 0; i < 10; ++i) {
        vec.emplace_back(5, i);
        vec.emplace_back(6, i);
    }

    // Sort on first field
    auto my_sort = randomizing_adapter<cppsort::smooth_sorter>{};
    my_sort(vec, &std::pair<int, int>::first);

    // Second field values should change between executions
    for (const auto& pair: vec) {
        std::cout << pair.first << '\t';
    }
    std::cout << '\n';for (const auto& pair: vec) {
        std::cout << pair.second << '\t';
    }
    std::cout << '\n';
}
