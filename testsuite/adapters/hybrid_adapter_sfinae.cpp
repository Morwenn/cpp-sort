/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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
#include <string>
#include <type_traits>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <testing-tools/span.h>

namespace
{
    // Type of sorter used for checks
    enum class sorter_type
    {
        integer,
        floating_point,
        generic
    };

    struct integer_sorter_impl
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> std::enable_if_t<
                std::is_integral<
                    typename std::iterator_traits<RandomAccessIterator>::value_type
                >::value,
                sorter_type
            >
        {
            return sorter_type::integer;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct float_sorter_impl
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> std::enable_if_t<
                std::is_floating_point<
                    typename std::iterator_traits<RandomAccessIterator>::value_type
                >::value,
                sorter_type
            >
        {
            return sorter_type::floating_point;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct generic_sorter_impl
    {
        template<typename RandomAccessIterator>
        auto operator()(RandomAccessIterator, RandomAccessIterator) const
            -> sorter_type
        {
            return sorter_type::generic;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    struct integer_sorter:
        cppsort::sorter_facade<integer_sorter_impl>
    {};

    struct float_sorter:
        cppsort::sorter_facade<float_sorter_impl>
    {};

    struct generic_sorter:
        cppsort::sorter_facade<generic_sorter_impl>
    {};
}

TEST_CASE( "sfinae forwarding in hybrid_adapter",
           "[hybrid_adapter][sfinae]" )
{
    // Check that hybrid_adapter takes into account
    // the SFINAE in the aggregated sorters

    // Vectors to "sort"
    std::vector<int> vec1(3);
    std::vector<float> vec2(3);
    std::vector<std::string> vec3(3);

    cppsort::hybrid_adapter<
        float_sorter,
        integer_sorter,
        // Should act as a fallback
        generic_sorter
    > sorter;

    SECTION( "with iterators" )
    {
        sorter_type res1 = sorter(std::begin(vec1), std::end(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(std::begin(vec2), std::end(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(std::begin(vec3), std::end(vec3));
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with iterables" )
    {
        sorter_type res1 = sorter(vec1);
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2);
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3);
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with span" )
    {
        sorter_type res1 = sorter(make_span(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(make_span(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(make_span(vec3));
        CHECK( res3 == sorter_type::generic );
    }
}

TEST_CASE( "sfinae forwarding in nested hybrid_adapter",
           "[hybrid_adapter][sfinae]" )
{
    // Check that hybrid_adapter takes into account
    // the SFINAE in the aggregated sorters

    // Vectors to "sort"
    std::vector<int> vec1(3);
    std::vector<float> vec2(3);
    std::vector<std::string> vec3(3);

    cppsort::hybrid_adapter<
        cppsort::hybrid_adapter<
            float_sorter,
            integer_sorter
        >,
        // Should act as a fallback
        generic_sorter
    > sorter;

    SECTION( "with iterators" )
    {
        sorter_type res1 = sorter(std::begin(vec1), std::end(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(std::begin(vec2), std::end(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(std::begin(vec3), std::end(vec3));
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with iterables" )
    {
        sorter_type res1 = sorter(vec1);
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(vec2);
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(vec3);
        CHECK( res3 == sorter_type::generic );
    }

    SECTION( "with span" )
    {
        sorter_type res1 = sorter(make_span(vec1));
        CHECK( res1 == sorter_type::integer );

        sorter_type res2 = sorter(make_span(vec2));
        CHECK( res2 == sorter_type::floating_point );

        sorter_type res3 = sorter(make_span(vec3));
        CHECK( res3 == sorter_type::generic );
    }
}
