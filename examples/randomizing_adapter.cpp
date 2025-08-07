/*
 * Copyright (c) 2022-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <random>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorters/smooth_sorter.h>
#include <cpp-sort/utility/adapter_storage.h>

template<typename RandomAccessIterator>
auto my_shuffle(RandomAccessIterator begin, RandomAccessIterator end)
    -> void
{
    thread_local std::random_device device;
    thread_local std::minstd_rand engine(device());
    std::shuffle(begin, end, engine);
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

    template<typename RandomAccessIterator, typename... Args>
    auto operator()(RandomAccessIterator begin, RandomAccessIterator end, Args&&... args) const
        -> decltype(this->get()(begin, end, std::forward<Args>(args)...))
    {
        static_assert(
            std::is_base_of_v<
                iterator_category,
                typename std::iterator_traits<RandomAccessIterator>::iterator_category
            >,
            "randomizing_adapter requires at least forward iterators"
        );

        my_shuffle(begin, end);
        return this->get()(begin, end, std::forward<Args>(args)...);
    }

    template<typename RandomAccessRange, typename... Args>
    auto operator()(RandomAccessRange&& range, Args&&... args) const
        -> decltype(this->get()(std::forward<RandomAccessRange>(range), std::forward<Args>(args)...))
    {
        static_assert(
            std::is_base_of_v<
                iterator_category,
                typename std::iterator_traits<decltype(std::begin(range))>::iterator_category
            >,
            "randomizing_adapter requires at least forward iterators"
        );

        my_shuffle(std::begin(range), std::end(range));
        return this->get()(std::forward<RandomAccessRange>(range), std::forward<Args>(args)...);
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
