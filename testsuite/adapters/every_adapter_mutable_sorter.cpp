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
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/utility/functional.h>
#include "../distributions.h"

namespace
{
    ////////////////////////////////////////////////////////////
    // Regular mutable sorter

    struct mutable_sorter_impl
    {
        int dummy1=0, dummy2=0;

        mutable_sorter_impl() = default;
        constexpr mutable_sorter_impl(int a, int b):
            dummy1(a), dummy2(b)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<
                cppsort::is_projection_iterator_v<Projection, Iterator, Compare>
            >
        >
        auto operator()(Iterator first, Iterator last,
                        Compare compare={}, Projection projection={})
            -> void
        {
            dummy1 = 3;
            cppsort::insertion_sort(std::move(first), std::move(last),
                                    std::move(compare), std::move(projection));
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


TEST_CASE( "test most adapters with a mutable sorter",
           "[adapters][adapter_storage]" )
{
    std::vector<int> collection;
    collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    // Sorter that will be adapted by everything
    auto original_sorter = mutable_sorter(5, 7);

    SECTION( "counting_adapter" )
    {
        auto adapted_sorter = cppsort::counting_adapter<mutable_sorter>(original_sorter);

        std::size_t res = adapted_sorter(collection);
        CHECK( res > 0 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    /*SECTION( "hybrid_adapter" )
    {
        auto adapted_sorter = cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            mutable_sorter
        >(cppsort::merge_sort, original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.template get<1>().dummy1 == 3 );
        CHECK( adapted_sorter.template get<1>().dummy2 == 11 );
    }*/

    SECTION( "indirect_adapter" )
    {
        auto adapted_sorter = cppsort::indirect_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "schwartz_adapter" )
    {
        auto adapted_sorter = cppsort::schwartz_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "self_sort_adapter" )
    {
        auto adapted_sorter = cppsort::self_sort_adapter<mutable_sorter>(original_sorter);

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        adapted_sorter(li);
        CHECK( std::is_sorted(std::begin(li), std::end(li)) );
        // List sorted itself without using the original sorter
        CHECK( adapted_sorter.get().dummy1 == 5 );
        CHECK( adapted_sorter.get().dummy2 == 7 );

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        // List sorted itself without using the original sorter
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "stable_adapter" )
    {
        auto adapted_sorter = cppsort::stable_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }

    SECTION( "verge_adapter" )
    {
        auto adapted_sorter = cppsort::verge_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(collection);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }
}
