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
#include <type_traits>
#include <utility>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/adapter_storage.h>
#include <cpp-sort/utility/functional.h>

namespace
{
    ////////////////////////////////////////////////////////////
    // Dummy adapter

    template<typename Sorter>
    struct dummy_adapter_impl:
        cppsort::utility::adapter_storage<Sorter>
    {
        dummy_adapter_impl() = default;

        constexpr dummy_adapter_impl(Sorter sorter):
            cppsort::utility::adapter_storage<Sorter>(std::move(sorter))
        {}

        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity
        >
        auto operator()(ForwardIterator first, ForwardIterator last,
                        Compare compare={}, Projection projection={}) const
            -> std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, ForwardIterator, Compare
            >>
        {
            cppsort::utility::adapter_storage<Sorter>::operator()(first, last, compare, projection);
        }

        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity
        >
        auto operator()(ForwardIterator first, ForwardIterator last,
                        Compare compare={}, Projection projection={})
            -> std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, ForwardIterator, Compare
            >>
        {
            cppsort::utility::adapter_storage<Sorter>::operator()(first, last, compare, projection);
        }
    };

    template<typename Sorter>
    struct dummy_adapter:
        cppsort::sorter_facade<dummy_adapter_impl<Sorter>>
    {
        dummy_adapter() = default;

        constexpr dummy_adapter(Sorter sorter):
            cppsort::sorter_facade<dummy_adapter_impl<Sorter>>(std::move(sorter))
        {}
    };

    ////////////////////////////////////////////////////////////
    // Immutable & non-empty sorter

    struct non_empty_sorter_impl
    {
        int dummy1=0, dummy2=0;

        non_empty_sorter_impl() = default;
        constexpr non_empty_sorter_impl(int a, int b):
            dummy1(a), dummy2(b)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename = std::enable_if_t<
                not cppsort::is_projection_iterator_v<Compare, Iterator>
            >
        >
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> void
        {
            std::sort(std::move(first), std::move(last), std::move(compare));
        }

        using iterator_category = std::random_access_iterator_tag;
        using is_always_stable = std::false_type;
    };

    struct non_empty_sorter:
        cppsort::sorter_facade<non_empty_sorter_impl>
    {
        non_empty_sorter() = default;
        non_empty_sorter(int a, int b):
            cppsort::sorter_facade<non_empty_sorter_impl>(a, b)
        {}
    };

    ////////////////////////////////////////////////////////////
    // Mutable sorter

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
            typename = std::enable_if_t<
                not cppsort::is_projection_iterator_v<Compare, Iterator>
            >
        >
        auto operator()(Iterator first, Iterator last, Compare compare={})
            -> void
        {
            dummy1 = 3;
            std::sort(std::move(first), std::move(last), std::move(compare));
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

TEST_CASE( "test correct adapter_storage behavior", "[adapter_storage]" )
{
    std::vector<int> arr = { 2, 8, 7, 4, 3, 6, 0, 1, 2, 8, 6, 9 };

    SECTION( "with an immutable empty sorter" )
    {
        cppsort::selection_sorter original_sorter{};
        auto adapted_sorter = dummy_adapter<cppsort::selection_sorter>(original_sorter);

        void(*my_sort)(std::vector<int>&) = adapted_sorter;
        my_sort(arr);
        CHECK( std::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with an immutable non-empty sorter" )
    {
        non_empty_sorter original_sorter(5, 8);
        auto adapted_sorter = dummy_adapter<non_empty_sorter>(original_sorter);

        adapted_sorter(arr);
        CHECK( std::is_sorted(std::begin(arr), std::end(arr)) );
    }

    SECTION( "with a mutable sorter" )
    {
        mutable_sorter original_sorter(5, 8);
        auto adapted_sorter = dummy_adapter<mutable_sorter>(original_sorter);

        adapted_sorter(arr);
        CHECK( std::is_sorted(std::begin(arr), std::end(arr)) );
        CHECK( adapted_sorter.get().dummy1 == 3 );
        CHECK( adapted_sorter.get().dummy2 == 11 );
    }
}
