/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorter_facade.h>

namespace
{
    struct return_sorter_impl
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
            -> int
        {
            return 42;
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    struct return_sorter_impl2
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
            -> int
        {
            return 43;
        }

        using iterator_category = std::forward_iterator_tag;
        using is_always_stable = std::true_type;
    };

    struct return_sorter:
        cppsort::sorter_facade<return_sorter_impl>
    {};

    struct return_sorter2:
        cppsort::sorter_facade<return_sorter_impl2>
    {};
}

TEST_CASE( "check that adapters correctly forward the result of the wrapped sorter",
           "[adapters]" )
{
    // Collections to "sort"
    std::vector<int> vec;
    std::list<int> li;

    SECTION( "container_aware_adapter" )
    {
        auto sort = cppsort::container_aware_adapter<
            return_sorter
        >{};
        CHECK( sort(vec) == 42 );

        // TODO: check with dedicated collection
    }

    SECTION( "hybrid_adapter" )
    {
        auto sort = cppsort::hybrid_adapter<
            return_sorter,
            return_sorter2
        >{};
        CHECK( sort(vec) == 42 );
        CHECK( sort(li) == 43 );
    }

    SECTION( "schwartz_adapter" )
    {
        auto sort = cppsort::schwartz_adapter<
            return_sorter
        >{};
        CHECK( sort(vec) == 42 );
        CHECK( sort(vec.begin(), vec.end(), std::less<>{}, cppsort::utility::identity{}) == 42 );
        CHECK( sort(vec.begin(), vec.end(), cppsort::utility::identity{}) == 42 );
    }

    SECTION( "self_sort_adapter" )
    {
        auto sort = cppsort::self_sort_adapter<
            return_sorter
        >{};
        CHECK( sort(vec) == 42 );

        // TODO: check with dedicated collection
    }

    // TODO: small_array_adapter

    SECTION( "stable_adapter" )
    {
        auto sort1 = cppsort::stable_adapter<
            return_sorter
        >{};
        CHECK( sort1(vec) == 42 );

        auto sort2 = cppsort::stable_adapter<
            return_sorter2
        >{};
        CHECK( sort2(vec) == 43 );
    }
}
