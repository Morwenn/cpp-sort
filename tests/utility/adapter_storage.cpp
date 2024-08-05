/*
 * Copyright (c) 2018-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/adapter_storage.h>

namespace
{
    ////////////////////////////////////////////////////////////
    // Dummy adapter

    template<typename Sorter>
    struct dummy_adapter_impl:
        cppsort::utility::adapter_storage<Sorter>
    {
        dummy_adapter_impl() = default;

        explicit constexpr dummy_adapter_impl(Sorter&& sorter):
            cppsort::utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_iterator_v<Projection, ForwardIterator, Compare>
        auto operator()(ForwardIterator first, ForwardIterator last,
                        Compare compare={}, Projection projection={}) const
            -> void
        {
            this->get()(first, last, compare, projection);
        }

        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_iterator_v<Projection, ForwardIterator, Compare>
        auto operator()(ForwardIterator first, ForwardIterator last,
                        Compare compare={}, Projection projection={})
            -> void
        {
            this->get()(first, last, compare, projection);
        }
    };

    template<typename Sorter>
    struct dummy_adapter:
        cppsort::sorter_facade<dummy_adapter_impl<Sorter>>
    {
        dummy_adapter() = default;

        explicit constexpr dummy_adapter(Sorter sorter):
            cppsort::sorter_facade<dummy_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // Immutable & non-empty sorter

    struct non_empty_sorter_impl
    {
        int dummy1=0, dummy2=0;

        non_empty_sorter_impl() = default;
        constexpr non_empty_sorter_impl(int a, int b):
            dummy1(a), dummy2(b)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>
        >
            requires (not cppsort::is_projection_iterator_v<Compare, Iterator>)
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> void
        {
            std::sort(std::move(first), std::move(last), std::move(compare));
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    struct non_empty_sorter:
        cppsort::sorter_facade<non_empty_sorter_impl>
    {
        non_empty_sorter() = default;
        non_empty_sorter(int a, int b):
            cppsort::sorter_facade<non_empty_sorter_impl>(a, b)
        {}
    };

    ////////////////////////////////////////////////////////////
    // Mutable sorter

    struct mutable_sorter_impl
    {
        int dummy1=0, dummy2=0;

        mutable_sorter_impl() = default;
        constexpr mutable_sorter_impl(int a, int b):
            dummy1(a), dummy2(b)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>
        >
            requires (not cppsort::is_projection_iterator_v<Compare, Iterator>)
        auto operator()(Iterator first, Iterator last, Compare compare={})
            -> void
        {
            dummy1 = 3;
            std::sort(std::move(first), std::move(last), std::move(compare));
            dummy2 = 11;
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    struct mutable_sorter:
        cppsort::sorter_facade<mutable_sorter_impl>
    {
        mutable_sorter() = default;
        mutable_sorter(int a, int b):
            cppsort::sorter_facade<mutable_sorter_impl>(a, b)
        {}
    };
}

TEST_CASE( "test correct adapter_storage behavior", "[adapter_storage]" )
{
    std::vector<int> arr = { 2, 8, 7, 4, 3, 6, 0, 1, 2, 8, 6, 9 };

    SECTION( "with an immutable empty sorter" )
    {
        cppsort::selection_sorter original_sorter{};
        auto adapted_sorter = dummy_adapter<cppsort::selection_sorter>(original_sorter);

        adapted_sorter(arr);
        CHECK( std::is_sorted(arr.begin(), arr.end()) );
    }

    SECTION( "with an immutable non-empty sorter" )
    {
        non_empty_sorter original_sorter(5, 8);
        auto adapted_sorter = dummy_adapter<non_empty_sorter>(original_sorter);

        adapted_sorter(arr);
        CHECK( std::is_sorted(arr.begin(), arr.end()) );
    }

    SECTION( "with a mutable sorter" )
    {
        mutable_sorter original_sorter(5, 8);
        auto adapted_sorter = dummy_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(arr);
        CHECK( std::is_sorted(std::begin(arr), std::end(arr)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }
}
