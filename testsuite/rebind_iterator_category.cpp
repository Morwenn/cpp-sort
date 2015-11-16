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
#include <forward_list>
#include <iterator>
#include <list>
#include <type_traits>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace
{
    enum struct sorter_type
    {
        foo,
        bar
    };

    struct foo_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::foo;
        }
    };

    struct bar_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> sorter_type
        {
            return sorter_type::bar;
        }
    };

    struct foo_sorter:
        cppsort::sorter_facade<foo_sorter_impl>
    {};

    struct bar_sorter:
        cppsort::sorter_facade<bar_sorter_impl>
    {};
}

namespace cppsort
{
    template<>
    struct sorter_traits<bar_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = true;
    };

    template<>
    struct sorter_traits<foo_sorter>
    {
        using iterator_category = std::forward_iterator_tag;
        static constexpr bool is_stable = false;
    };
}

TEST_CASE( "iterator category rebinder",
           "[rebind_iterator_category][hybrid_adapter]" )
{
    SECTION( "simple case" )
    {
        using sorter1 = cppsort::rebind_iterator_category<
            foo_sorter,
            std::bidirectional_iterator_tag
        >;

        using sorter2 = cppsort::rebind_iterator_category<
            bar_sorter,
            std::random_access_iterator_tag
        >;

        CHECK( (std::is_same<cppsort::iterator_category<sorter1>, std::bidirectional_iterator_tag>::value) );
        CHECK( (std::is_same<cppsort::iterator_category<sorter2>, std::random_access_iterator_tag>::value) );
    }

    SECTION( "with hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            foo_sorter,
            cppsort::rebind_iterator_category<
                bar_sorter,
                std::bidirectional_iterator_tag
            >
        >;

        std::forward_list<float> collection1(3);
        sorter_type res1 = cppsort::sort(collection1, sorter{});
        CHECK( res1 == sorter_type::foo );

        std::list<float> collection2(3);
        sorter_type res2 = cppsort::sort(collection2, sorter{});
        CHECK( res2 == sorter_type::bar );

        std::vector<float> collection3(3);
        sorter_type res3 = cppsort::sort(collection3, sorter{});
        CHECK( res3 == sorter_type::bar );
    }
}
