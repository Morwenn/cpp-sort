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
#include <iterator>
#include <list>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include "../span.h"

namespace
{
    enum class sorter_type
    {
        bidirectional,
        random_access
    };

    template<int N>
    struct random_access_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::random_access;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct bidirectional_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::bidirectional;
        }

        using iterator_category = std::bidirectional_iterator_tag;
    };

    template<int N>
    struct random_access_sorter:
        cppsort::sorter_facade<random_access_sorter_impl<N>>
    {};

    struct bidirectional_sorter:
        cppsort::sorter_facade<bidirectional_sorter_impl>
    {};
}

TEST_CASE( "hybrid_adapter with many sorters",
           "[hybrid_adapter]" )
{
    // Check that hybrid_adapter scales even when there are
    // a great many sorters involved, especially check that
    // the indices scale well

    cppsort::hybrid_adapter<
        bidirectional_sorter,
        random_access_sorter<0>,
        random_access_sorter<1>,
        random_access_sorter<2>,
        random_access_sorter<3>,
        random_access_sorter<4>,
        random_access_sorter<5>
    > sorter;

    std::vector<int> vec;
    auto res1 = sorter(vec);
    CHECK( res1 == sorter_type::random_access );

    std::list<int> li;
    auto res2 = sorter(li);
    CHECK( res2 == sorter_type::bidirectional );
}
