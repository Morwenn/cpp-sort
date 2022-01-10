/*
 * Copyright (c) 2021-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/as_function.h>
#include <cpp-sort/utility/functional.h>
#include <cpp-sort/utility/iter_move.h>
#include <testing-tools/algorithm.h>

namespace
{
    struct constexpr_insertion_sorter_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<
                cppsort::is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
            >
        >
        constexpr auto operator()(RandomAccessIterator first, RandomAccessIterator last,
                                  Compare compare={}, Projection projection={}) const
            -> void
        {
            if (first == last) return;

            using cppsort::utility::iter_move;
            auto&& comp = cppsort::utility::as_function(compare);
            auto&& proj = cppsort::utility::as_function(projection);

            for (auto cur = first + 1 ; cur != last ; ++cur) {
                auto sift = cur;
                auto sift_1 = sift - 1;

                // Compare first so we can avoid 2 moves for
                // an element already positioned correctly.
                if (comp(proj(*sift), proj(*sift_1))) {
                    auto tmp = iter_move(sift);
                    auto&& tmp_proj = proj(tmp);

                    do {
                        *sift = iter_move(sift_1);
                    } while (--sift != first && comp(tmp_proj, proj(*--sift_1)));
                    *sift = std::move(tmp);
                }
            }
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::true_type;
    };

    struct constexpr_insertion_sorter:
        cppsort::sorter_facade<constexpr_insertion_sorter_impl>
    {};

    constexpr auto test_sorter()
        -> bool
    {
        constexpr std::size_t size = 13;
        int collection[size] = { 15, 6, 0, 2, 2, 3, 8, 12, 10, 5, 9, 7, 10 };
        constexpr_insertion_sorter sorter;
        sorter(collection, collection + size);
        return helpers::is_sorted(collection, collection + size);
    }
}

TEST_CASE( "test basic constexpr support", "[sorter_facade][constexpr]" )
{
    // Check that sorter_facade can be useful in a constexpr constext
    // if the sorter implementation is constexpr-friendly itself
    constexpr bool is_sorted = test_sorter();
    STATIC_CHECK( is_sorted );
}
