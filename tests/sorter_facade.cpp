/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <functional>
#include <type_traits>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/wrapper.h>

namespace
{
    struct comparison_sorter_impl
    {
        template<typename Iterator, typename Compare=std::less<>>
        auto operator()(Iterator, Iterator, Compare={}) const
            -> bool
        {
            return true;
        }
    };

    struct projection_sorter_impl
    {
        template<typename Iterator, typename Projection=cppsort::utility::identity>
        auto operator()(Iterator, Iterator, Projection={}) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_projection_sorter_impl
    {
        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, Iterator, Compare
            >>
        >
        auto operator()(Iterator, Iterator, Compare={}, Projection={}) const
            -> bool
        {
            return true;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};

    struct projection_sorter:
        cppsort::sorter_facade<projection_sorter_impl>
    {};

    struct comparison_projection_sorter:
        cppsort::sorter_facade<comparison_projection_sorter_impl>
    {};
}

TEST_CASE( "sorter_facade miscellaneous checks",
           "[sorter_facade][comparison][projection]" )
{
    // Some checks to make sure that sorter_facade always
    // forwards the value correctly in the most common cases

    using wrapper = generic_wrapper<int>;

    // Collection to "sort"
    std::vector<int> vec;
    std::vector<wrapper> vec_wrap;

    SECTION( "with comparison only" )
    {
        CHECK( comparison_sorter{}(vec, std::less<>{}) );
        CHECK( comparison_sorter{}(vec.begin(), vec.end(), std::less<>{}) );

        CHECK( comparison_sorter{}(vec, std::greater<>{}) );
        CHECK( comparison_sorter{}(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "with projection only" )
    {
        CHECK( projection_sorter{}(vec, cppsort::utility::identity{}) );
        CHECK( projection_sorter{}(vec.begin(), vec.end(), cppsort::utility::identity{}) );

        CHECK( projection_sorter{}(vec_wrap, &wrapper::value) );
        CHECK( projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value) );
    }

    SECTION( "with both comparison and projection" )
    {
        CHECK( comparison_projection_sorter{}(vec, std::less<>{}) );
        CHECK( comparison_projection_sorter{}(vec.begin(), vec.end(), std::less<>{}) );

        CHECK( comparison_projection_sorter{}(vec, std::greater<>{}) );
        CHECK( comparison_projection_sorter{}(vec.begin(), vec.end(), std::greater<>{}) );

        CHECK( comparison_projection_sorter{}(vec, cppsort::utility::identity{}) );
        CHECK( comparison_projection_sorter{}(vec.begin(), vec.end(), cppsort::utility::identity{}) );

        CHECK( comparison_projection_sorter{}(vec_wrap, &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(), &wrapper::value) );

        CHECK( comparison_projection_sorter{}(vec, std::greater<>{}, cppsort::utility::identity{}) );
        CHECK( comparison_projection_sorter{}(vec.begin(), vec.end(),
                                              std::greater<>{}, cppsort::utility::identity{}) );

        CHECK( comparison_projection_sorter{}(vec_wrap, std::greater<>{}, &wrapper::value) );
        CHECK( comparison_projection_sorter{}(vec_wrap.begin(), vec_wrap.end(),
                                              std::greater<>{}, &wrapper::value) );
    }
}
