/*
 * Copyright (c) 2016-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>
#include <cpp-sort/sorters.h>

template<template<typename...> class Collection, typename T, typename Compare>
using sort_f = void (*)(Collection<T>&, Compare);

struct compare_t
{
    // Count copies and moves
    int& copies;
    int& moves;

    // Ensure that the class isn't ever used after a move
    bool has_moved = false;

    compare_t(int& copies, int& moves):
        copies(copies),
        moves(moves)
    {}

    compare_t(const compare_t& other):
        copies(other.copies),
        moves(other.moves)
    {
        copies += 1;
    }

    compare_t(compare_t&& other):
        copies(other.copies),
        moves(other.moves)
    {
        if (std::exchange(other.has_moved, true)) {
            throw std::logic_error("illegal read from a moved-from value");
        }
        moves += 1;
    }

    // Ensure that algorithms accept copy-constructible comparisons
    compare_t& operator=(const compare_t&) = delete;
    compare_t& operator=(compare_t&&) = delete;

    template<typename T, typename U>
    auto operator()(T&& lhs, U&& rhs)
        -> decltype(std::forward<T>(lhs) < std::forward<U>(rhs))
    {
        if (has_moved) {
            throw std::logic_error("illegal read from a moved-from value");
        }
        return std::forward<T>(lhs) < std::forward<U>(rhs);
    }
};

int main()
{
    using namespace std::literals;

    std::pair<std::string, sort_f<std::vector, int, compare_t>> sorts[] = {
        { "grail_sort",             cppsort::grail_sort             },
        { "heap_sort",              cppsort::heap_sort              },
        { "insertion_sort",         cppsort::insertion_sort         },
        { "merge_insertion_sort",   cppsort::merge_insertion_sort   },
        { "merge_sort",             cppsort::merge_sort             },
        { "pdq_sort",               cppsort::pdq_sort               },
        { "poplar_sort",            cppsort::poplar_sort            },
        { "quick_sort",             cppsort::quick_sort             },
        { "selection_sort",         cppsort::selection_sort         },
        { "smooth_sort",            cppsort::smooth_sort            },
        { "std_sort",               cppsort::std_sort               },
        { "tim_sort",               cppsort::tim_sort               },
        { "verge_sort",             cppsort::verge_sort             },
        { "wiki_sort",              cppsort::wiki_sort              },
    };

    for (auto& sort_item: sorts) {
        auto&& name = sort_item.first;
        auto&& sort = sort_item.second;

        // Initialize collection to sort
        std::vector<int> collection(1'000, 0);
        std::iota(std::begin(collection), std::end(collection), 0);
        std::mt19937 engine(1477332479); // constant seed
        std::shuffle(std::begin(collection), std::end(collection), engine);

        // Initialize comparator
        int copies = 0;
        int moves = 0;
        compare_t compare(copies, moves);

        // Perform the sort
        sort(collection, compare);
        std::cout << std::left << std::setw(25) << (name + ":"s)
                  << std::left << std::setw(10) << copies << " copies,\t"
                  << std::left << std::setw(10) << moves << " moves,\t"
                  << std::left << std::setw(10) << copies + moves << "total\n";
    }
}
