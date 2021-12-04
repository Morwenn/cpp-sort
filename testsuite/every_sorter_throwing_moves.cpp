/*
 * Copyright (c) 2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <deque>
#include <forward_list>
#include <iterator>
#include <list>
#include <stdexcept>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>

namespace
{
    constexpr int throwing_move_value = 150;

    struct throw_on_move_error:
        std::logic_error
    {
        using std::logic_error::logic_error;
    };

    int correctly_constructed = 0;
    int correctly_destructed = 0;
    int moves_count = 0;

    struct throw_on_move
    {
        int value = 0;

        throw_on_move() = default;

        throw_on_move(int value):
            value(value)
        {
            ++correctly_constructed;
        }

        throw_on_move(const throw_on_move&) = delete;
        throw_on_move& operator=(throw_on_move&) = delete;

        throw_on_move(throw_on_move&& other):
            value(other.value)
        {
            if (++moves_count == throwing_move_value) {
                throw throw_on_move_error("move constructor threw");
            }
            ++correctly_constructed;
        }

        auto operator=(throw_on_move&& other)
            -> throw_on_move&
        {
            value = other.value;
            if (++moves_count == throwing_move_value) {
                throw throw_on_move_error("move operator threw");
            }
            return *this;
        }

        ~throw_on_move()
        {
            ++correctly_destructed;
        }

        friend auto operator<(const throw_on_move& lhs, const throw_on_move& rhs)
            -> bool
        {
            return lhs.value < rhs.value;
        }
    };
}

TEMPLATE_TEST_CASE( "random-access sorters against throwing move operations", "[sorters][throwing_moves]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    auto distribution = dist::shuffled{};
    // Initialize counters
    correctly_constructed = 0;
    correctly_destructed = 0;
    moves_count = 0;

    SECTION( "with std::vector" )
    {
        try {
            std::vector<throw_on_move> collection; collection.reserve(250);
            distribution(std::back_inserter(collection), 250);

            TestType sorter;
            sorter(collection);

            INFO( "the sorter did not throw" );
        } catch (const throw_on_move_error&) {
            INFO( "the sorter did throw" );
            CHECK( correctly_constructed == correctly_destructed );
        }
    }

    SECTION( "with std::deque" )
    {
        try {
            std::deque<throw_on_move> collection;
            distribution(std::back_inserter(collection), 250);

            TestType sorter;
            sorter(collection);

            INFO( "the sorter did not throw" );
        } catch (const throw_on_move_error&) {
            INFO( "the sorter did throw" );
            CHECK( correctly_constructed == correctly_destructed );
        }
    }
}

TEMPLATE_TEST_CASE( "bidirectional sorters against throwing move operations", "[sorters][throwing_moves]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::verge_sorter )
{
    auto distribution = dist::shuffled{};
    // Initialize counters
    correctly_constructed = 0;
    correctly_destructed = 0;
    moves_count = 0;

    try {
        std::list<throw_on_move> collection;
        distribution(std::back_inserter(collection), 250);

        TestType sorter;
        sorter(collection);

        INFO( "the sorter did not throw" );
    } catch (const throw_on_move_error&) {
        INFO( "the sorter did throw" );
        CHECK( correctly_constructed == correctly_destructed );
    }
}

TEMPLATE_TEST_CASE( "forward sorters against throwing move operations", "[sorters][throwing_moves]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    auto distribution = dist::shuffled{};
    // Initialize counters
    correctly_constructed = 0;
    correctly_destructed = 0;
    moves_count = 0;

    try {
        std::forward_list<throw_on_move> collection;
        distribution(std::front_inserter(collection), 250);

        TestType sorter;
        sorter(collection);

        INFO( "the sorter did not throw" );
    } catch (const throw_on_move_error&) {
        INFO( "the sorter did throw" );
        CHECK( correctly_constructed == correctly_destructed );
    }
}
