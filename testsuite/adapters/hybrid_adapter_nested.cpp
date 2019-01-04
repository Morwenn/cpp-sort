/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <vector>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <catch2/catch.hpp>

// Type of sorter used for checks
enum class sorter_type
{
    forward,
    bidirectional,
    random_access
};

struct forward_sorter_impl
{
    template<typename ForwardIterator>
    auto operator()(ForwardIterator, ForwardIterator) const
    -> sorter_type
    {
        return sorter_type::forward;
    }

    using iterator_category = std::forward_iterator_tag;
};

struct forward_sorter : cppsort::sorter_facade<forward_sorter_impl> {};

struct bidirectional_sorter_impl
{
    template<typename BidirectionalIterator>
    auto operator()(BidirectionalIterator, BidirectionalIterator) const
    -> sorter_type
    {
        return sorter_type::bidirectional;
    }

    using iterator_category = std::bidirectional_iterator_tag;
};
struct bidirectional_sorter : cppsort::sorter_facade<bidirectional_sorter_impl> {};

struct random_access_sorter_impl
{
    template<typename RandomAccessIterator>
    auto operator()(RandomAccessIterator, RandomAccessIterator) const
    -> sorter_type
    {
        return sorter_type::random_access;
    }

    using iterator_category = std::random_access_iterator_tag;
};

struct random_access_sorter : cppsort::sorter_facade<random_access_sorter_impl> {};

TEST_CASE( "nested hybrid_adapter types", "[hybrid_adapter]" )
{
    // Make sure that the nesting of hybrid_adapter
    // does not hide more suitable sorters because
    // of the iterator category merging

    // Collections to sort
    std::vector<int> vec(3);
    std::list<int> list(3);

    SECTION( "unwrapping from the front" )
    {
        cppsort::hybrid_adapter<
                cppsort::hybrid_adapter<
                        forward_sorter,
                        random_access_sorter
                >,
                bidirectional_sorter
        > sorter;

        sorter_type res1 = sorter(vec);
        CHECK( res1 == sorter_type::random_access );

        sorter_type res2 = sorter(list);
        CHECK( res2 == sorter_type::bidirectional );
    }

    SECTION( "unwrapping from the back" )
    {
        cppsort::hybrid_adapter<
                bidirectional_sorter,
                cppsort::hybrid_adapter<
                        forward_sorter,
                        random_access_sorter
                >
        > sorter;

        sorter_type res = sorter(vec);
        CHECK( res == sorter_type::random_access );

        sorter_type res2 = sorter(list);
        CHECK( res2 == sorter_type::bidirectional );
    }
}
