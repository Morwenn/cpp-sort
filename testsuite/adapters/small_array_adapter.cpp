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
#include <array>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

namespace
{
    enum struct sorter_type
    {
        regular,
        fixed_with_domain,
        fixed_without_domain
    };

    template<std::size_t N>
    struct fixed_sorter_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator, Compare={}) const
            -> sorter_type
        {
            return sorter_type::fixed_without_domain;
        }
    };

    template<std::size_t N>
    struct fixed_sorter_with_domain_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator,
                        Compare={}, Projection={}) const
            -> sorter_type
        {
            return sorter_type::fixed_with_domain;
        }
    };

    struct regular_sorter_impl
    {
        template<
            typename RandomAccessIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
        auto operator()(RandomAccessIterator, RandomAccessIterator,
                        Compare={}, Projection={}) const
            -> sorter_type
        {
            return sorter_type::regular;
        }

        using iterator_category = std::random_access_iterator_tag;
    };

    template<std::size_t N>
    struct fixed_sorter:
        cppsort::sorter_facade<fixed_sorter_impl<N>>
    {};

    template<std::size_t N>
    struct fixed_sorter_with_domain:
        cppsort::sorter_facade<fixed_sorter_with_domain_impl<N>>
    {};

    struct regular_sorter:
        cppsort::sorter_facade<regular_sorter_impl>
    {};
}

namespace cppsort
{
    template<>
    struct fixed_sorter_traits<fixed_sorter>
    {
        using iterator_category = std::random_access_iterator_tag;
    };

    template<>
    struct fixed_sorter_traits<fixed_sorter_with_domain>
    {
        using domain = std::make_index_sequence<9>;
        using iterator_category = std::random_access_iterator_tag;
    };
}

TEST_CASE( "small array adapter",
           "[small_array_adapter][hybrid_adapter]" )
{
    std::array<int, 6u> array;
    std::array<int, 25u> big_array;

    SECTION( "without explicit indices not domain" )
    {
        cppsort::small_array_adapter<fixed_sorter> sorter;

        auto res1 = sorter(array);
        CHECK( res1 == sorter_type::fixed_without_domain );
        auto res2 = sorter(big_array);
        CHECK( res2 == sorter_type::fixed_without_domain );
    }

    SECTION( "with domain without explicit indices" )
    {
        cppsort::small_array_adapter<fixed_sorter_with_domain> sorter;

        auto res = sorter(array);
        CHECK( res == sorter_type::fixed_with_domain );
    }

    SECTION( "with explicit indices" )
    {
        cppsort::small_array_adapter<
            fixed_sorter,
            std::make_index_sequence<14u>
        > sorter;

        auto res = sorter(array);
        CHECK( res == sorter_type::fixed_without_domain );
    }

    SECTION( "with indices in hybrid_adapter" )
    {
        cppsort::hybrid_adapter<
            cppsort::small_array_adapter<
                fixed_sorter,
                std::make_index_sequence<14u>
            >,
            regular_sorter
        > sorter;

        auto res1 = sorter(array);
        CHECK( res1 == sorter_type::fixed_without_domain );
        auto res2 = sorter(big_array);
        CHECK( res2 == sorter_type::regular );
    }

    SECTION( "with domain in hybrid_adapter" )
    {
        cppsort::hybrid_adapter<
            cppsort::small_array_adapter<fixed_sorter_with_domain>,
            regular_sorter
        > sorter;

        auto res1 = sorter(array);
        CHECK( res1 == sorter_type::fixed_with_domain );
        auto res2 = sorter(big_array);
        CHECK( res2 == sorter_type::regular );
    }

    SECTION( "with domain and projections" )
    {
        struct wrapper { int value; };
        std::array<wrapper, 7> collection;

        cppsort::small_array_adapter<
            fixed_sorter_with_domain
        > sorter;

        auto res1 = sorter(collection, std::greater<>{}, &wrapper::value);
        CHECK( res1 == sorter_type::fixed_with_domain );
        auto res2 = sorter(collection, &wrapper::value);
        CHECK( res2 == sorter_type::fixed_with_domain );
    }
}
