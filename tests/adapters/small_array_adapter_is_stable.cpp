/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace
{
    template<std::size_t N>
    struct fixed_sorter_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator, Compare={}) const
            -> void
        {}

        using is_always_stable = std::false_type;
    };

    template<std::size_t N>
    struct fixed_sorter_with_domain_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = std::identity
        >
            requires cppsort::is_projection_iterator_v<Projection, RandomAccessIterator, Compare>
        auto operator()(RandomAccessIterator, RandomAccessIterator,
                        Compare={}, Projection={}) const
            -> void
        {}

        using is_always_stable = std::true_type;
    };

    template<std::size_t N>
    struct fixed_sorter:
        cppsort::sorter_facade<fixed_sorter_impl<N>>
    {};

    template<std::size_t N>
    struct fixed_sorter_with_domain:
        cppsort::sorter_facade<fixed_sorter_with_domain_impl<N>>
    {};
}

namespace cppsort
{
    template<>
    struct fixed_sorter_traits<fixed_sorter>
    {
        using is_always_stable = std::false_type;
        using iterator_category = std::random_access_iterator_tag;
    };

    template<>
    struct fixed_sorter_traits<fixed_sorter_with_domain>
    {
        using domain = std::make_index_sequence<9>;
        using is_always_stable = std::true_type;
        using iterator_category = std::random_access_iterator_tag;
    };
}

TEST_CASE( "small_array_adapter stability",
           "[small_array_adapter][schwartz_adapter][is_stable]" )
{
    std::array<int, 6u> array;
    int big_array[25u];

    using sorter1 = cppsort::small_array_adapter<fixed_sorter>;
    using sorter2 = cppsort::small_array_adapter<fixed_sorter_with_domain>;

    SECTION( "is_always_stable" )
    {
        STATIC_CHECK( not cppsort::is_always_stable<sorter1>::value );
        STATIC_CHECK( cppsort::is_always_stable<sorter2>::value );
    }

    SECTION( "is_stable" )
    {
        STATIC_CHECK( not cppsort::is_stable<sorter1(decltype(array))>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter1(decltype(big_array))>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter1(decltype(array), std::less<>)>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter1(decltype(big_array), std::less<>)>::value );

        STATIC_CHECK( cppsort::is_stable<sorter2(decltype(array))>::value );
        STATIC_CHECK( cppsort::is_stable<sorter2(decltype(array), std::less<>)>::value );
        STATIC_CHECK( cppsort::is_stable<sorter2(decltype(array), std::negate<>)>::value );
    }

    SECTION( "is_stable with schwartz_adapter" )
    {
        using sorter3 = cppsort::schwartz_adapter<sorter1>;
        using sorter4 = cppsort::schwartz_adapter<sorter2>;

        STATIC_CHECK( not cppsort::is_stable<sorter3(decltype(array))>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter3(decltype(big_array))>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter3(decltype(array), std::less<>)>::value );
        STATIC_CHECK( not cppsort::is_stable<sorter3(decltype(big_array), std::less<>)>::value );

        STATIC_CHECK( cppsort::is_stable<sorter4(decltype(array))>::value );
        STATIC_CHECK( cppsort::is_stable<sorter4(decltype(array), std::less<>)>::value );
        STATIC_CHECK( cppsort::is_stable<sorter4(decltype(array), std::negate<>)>::value );
    }
}
