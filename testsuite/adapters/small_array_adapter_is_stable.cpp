/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#include <cpp-sort/adapters/schwartz_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/utility/functional.h>

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
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, RandomAccessIterator, Compare
            >>
        >
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
        CHECK( not cppsort::is_always_stable<sorter1>::value );
        CHECK( cppsort::is_always_stable<sorter2>::value );
    }

    SECTION( "is_stable" )
    {
        CHECK( not cppsort::is_stable<sorter1(decltype(array))>::value );
        CHECK( not cppsort::is_stable<sorter1(decltype(big_array))>::value );
        CHECK( not cppsort::is_stable<sorter1(decltype(array), std::less<>)>::value );
        CHECK( not cppsort::is_stable<sorter1(decltype(big_array), std::less<>)>::value );

        CHECK( cppsort::is_stable<sorter2(decltype(array))>::value );
        CHECK( cppsort::is_stable<sorter2(decltype(array), std::less<>)>::value );
        CHECK( cppsort::is_stable<sorter2(decltype(array), std::negate<>)>::value );
    }

    SECTION( "is_stable with schwartz_adapter" )
    {
        using sorter3 = cppsort::schwartz_adapter<sorter1>;
        using sorter4 = cppsort::schwartz_adapter<sorter2>;

        CHECK( not cppsort::is_stable<sorter3(decltype(array))>::value );
        CHECK( not cppsort::is_stable<sorter3(decltype(big_array))>::value );
        CHECK( not cppsort::is_stable<sorter3(decltype(array), std::less<>)>::value );
        CHECK( not cppsort::is_stable<sorter3(decltype(big_array), std::less<>)>::value );

        CHECK( cppsort::is_stable<sorter4(decltype(array))>::value );
        CHECK( cppsort::is_stable<sorter4(decltype(array), std::less<>)>::value );
        CHECK( cppsort::is_stable<sorter4(decltype(array), std::negate<>)>::value );
    }
}
