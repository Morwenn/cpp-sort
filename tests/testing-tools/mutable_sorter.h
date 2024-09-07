/*
 * Copyright (c) 2029-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_MUTABLE_SORTER_H_
#define CPPSORT_TESTSUITE_MUTABLE_SORTER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/splay_sorter.h>

enum struct mutable_state
{
    initial,
    modified
};

struct mutable_sorter_impl
{
    mutable_state before_sort = mutable_state::initial;
    mutable_state after_sort = mutable_state::initial;

    mutable_sorter_impl() = default;

    constexpr mutable_sorter_impl(mutable_state before, mutable_state after):
        before_sort(before),
        after_sort(after)
    {}

    template<
        typename Iterator,
        typename Compare = std::less<>,
        typename Projection = std::identity,
        typename = std::enable_if_t<
            cppsort::is_projection_iterator_v<Projection, Iterator, Compare>
        >
    >
    auto operator()(Iterator first, Iterator last,
                    Compare compare={}, Projection projection={})
        -> void
    {
        before_sort = mutable_state::modified;
        cppsort::splay_sort(std::move(first), std::move(last),
                            std::move(compare), std::move(projection));
        after_sort = mutable_state::modified;
    }

    using iterator_category = std::forward_iterator_tag;
    using is_always_stable = std::false_type;
};

struct mutable_sorter:
    cppsort::sorter_facade<mutable_sorter_impl>
{
    mutable_sorter() = default;
    mutable_sorter(mutable_state before, mutable_state after):
        cppsort::sorter_facade<mutable_sorter_impl>(before, after)
    {}
};

#endif // CPPSORT_TESTSUITE_MUTABLE_SORTER_H_
