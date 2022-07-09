/*
 * Copyright (c) 2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <map>
#include <utility>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/comparators/projection_compare.h>
#include <testing-tools/is_transparent.h>

namespace
{
    // identity-like, but avoids special cases

    struct fake_identity
    {
        template<typename T>
        constexpr auto operator()(T&& value) const noexcept
            -> T&&
        {
            return std::forward<T>(value);
        }
    };

    struct fake_identity_transparent
    {
        template<typename T>
        constexpr auto operator()(T&& value) const noexcept
            -> T&&
        {
            return std::forward<T>(value);
        }

        using is_transparent = void;
    };
}

TEST_CASE( "is_transparent over projection_compare", "[comparison][is_transparent]" )
{
    using cmp1 = cppsort::projection_compare_t<std::less<is_transparent_helper_stored>, fake_identity>;
    using cmp2 = cppsort::projection_compare_t<std::less<is_transparent_helper_stored>, fake_identity_transparent>;
    using cmp3 = cppsort::projection_compare_t<std::less<>, fake_identity>;
    using cmp4 = cppsort::projection_compare_t<std::less<>, fake_identity_transparent>;

    STATIC_CHECK_FALSE( cppsort::detail::has_is_transparent_v<cmp1> );
    STATIC_CHECK_FALSE( cppsort::detail::has_is_transparent_v<cmp2> );
    STATIC_CHECK_FALSE( cppsort::detail::has_is_transparent_v<cmp3> );
    STATIC_CHECK( cppsort::detail::has_is_transparent_v<cmp4> );

    std::map<is_transparent_helper_stored, int, cmp1> mapping1 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_THROWS( mapping1.find(is_transparent_helper_compared(2)) );

    std::map<is_transparent_helper_stored, int, cmp2> mapping2 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_THROWS( mapping2.find(is_transparent_helper_compared(2)) );

    std::map<is_transparent_helper_stored, int, cmp3> mapping3 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_THROWS( mapping3.find(is_transparent_helper_compared(2)) );

    std::map<is_transparent_helper_stored, int, cmp4> mapping4 = {
        {1, 1},
        {2, 2},
        {3, 3},
    };
    CHECK_NOTHROW( mapping4.find(is_transparent_helper_compared(2)) );
}
