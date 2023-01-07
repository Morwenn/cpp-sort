/*
 * Copyright (c) 2016-2023 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <functional>
#include <iterator>
#include <list>
#include <stdexcept>
#include <type_traits>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/insertion_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

namespace
{
    struct throwing_sorter_impl
    {
        template<
            typename ForwardIterator,
            typename Compare = std::less<>,
            typename Projection = cppsort::utility::identity,
            typename = std::enable_if_t<cppsort::is_projection_iterator_v<
                Projection, ForwardIterator, Compare
            >>
        >
        auto operator()(ForwardIterator, ForwardIterator, Compare={}, Projection={}) const
            -> void
        {
            throw std::logic_error("throwing_sorter has been used");
        }

        using iterator_category = std::forward_iterator_tag;
        using is_always_stable = std::true_type;
    };

    struct throwing_sorter:
        cppsort::sorter_facade<throwing_sorter_impl>
    {};
}

TEST_CASE( "hybrid_adapter over adapters that change iterator category",
           "[hybrid_adapter][indirect_adapter][out_of_place_adapter]" )
{
    // In these tests we sort an instance of std::list with a sorter
    // that can only sort random-access iterators wrapped in an adapter
    // that changes that property: typically the adapter performs some
    // kind of out-of-place sort over a memory buffer

    std::list<int> li;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(li), 100, 10'000);

    SECTION( "hybrid_adapter over indirect_adapter" )
    {
        cppsort::hybrid_adapter<
            cppsort::indirect_adapter<
                cppsort::poplar_sorter
            >,
            throwing_sorter
        > sorter;

        sorter(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }

    SECTION( "hybrid_adapter over out_of_place_adapter" )
    {
        cppsort::hybrid_adapter<
            cppsort::out_of_place_adapter<
                cppsort::poplar_sorter
            >,
            throwing_sorter
        > sorter;

        sorter(li.begin(), li.end());
        CHECK( std::is_sorted(li.begin(), li.end()) );
    }
}

TEST_CASE( "indirect sort with Schwartzian transform",
           "[indirect_adapter][schwartz_adapter]" )
{
    using wrapper = generic_wrapper<short>;

    std::vector<wrapper> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 334, -93);

    SECTION( "schwartz_adapter over indirect_adapter" )
    {
        cppsort::schwartz_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over schwartz_adapter" )
    {
        cppsort::indirect_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "schwartz_adapter over schwartz_adapter" )
    {
        cppsort::schwartz_adapter<
            cppsort::schwartz_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }

    SECTION( "indirect_adapter over indirect_adapter" )
    {
        cppsort::indirect_adapter<
            cppsort::indirect_adapter<
                cppsort::selection_sorter
            >
        > sorter;

        sorter(collection, &wrapper::value);
        CHECK( helpers::is_sorted(collection.begin(), collection.end(),
                                  std::less<>{}, &wrapper::value) );
    }
}

TEST_CASE( "stability of counting_adapter over self_sort_adapter",
           "[counting_adapter][self_sort_adapter][is_stable]" )
{
    // Big checks to ensure that mixed sorters have a valid stability

    using sorter1 = cppsort::counting_adapter<
        cppsort::self_sort_adapter<
            cppsort::selection_sorter
        >
    >;

    using sorter2 = cppsort::counting_adapter<
        cppsort::self_sort_adapter<
            cppsort::insertion_sorter
        >
    >;

    SECTION( "is_always_stable" )
    {
        STATIC_CHECK( not cppsort::is_always_stable<sorter1>::value );
        STATIC_CHECK( not cppsort::is_always_stable<sorter2>::value );
    }

    SECTION( "is_stable" )
    {
        using cppsort::is_stable;

        STATIC_CHECK( is_stable<sorter1(std::list<int>&)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>&)>::value );
        STATIC_CHECK( is_stable<sorter1(std::list<int>&, std::greater<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>&, std::greater<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::list<int>&, std::negate<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>&, std::negate<>)>::value );

        STATIC_CHECK( not is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator, std::greater<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator, std::greater<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::list<int>::iterator, std::list<int>::iterator, std::negate<>)>::value );
        STATIC_CHECK( not is_stable<sorter1(std::vector<int>::iterator, std::vector<int>::iterator, std::negate<>)>::value );

        STATIC_CHECK( is_stable<sorter2(std::list<int>&)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>&)>::value );
        STATIC_CHECK( is_stable<sorter2(std::list<int>&, std::greater<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>&, std::greater<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::list<int>&, std::negate<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>&, std::negate<>)>::value );

        STATIC_CHECK( is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator)>::value );
        STATIC_CHECK( is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator, std::greater<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator, std::greater<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::list<int>::iterator, std::list<int>::iterator, std::negate<>)>::value );
        STATIC_CHECK( is_stable<sorter2(std::vector<int>::iterator, std::vector<int>::iterator, std::negate<>)>::value );
    }
}

TEST_CASE( "stable_adapter over stable_adapter", "[stable_adapter]" )
{
    // Wrap/nest stable_adapter several times and check that
    // stable_t always returns the most nested stable sorter

    SECTION( "over unstable sorter" )
    {
        using sorter = cppsort::stable_adapter<cppsort::selection_sorter>;
        using nested1 = cppsort::stable_adapter<sorter>;
        using nested2 = cppsort::stable_adapter<nested1>;
        using nested3 = cppsort::stable_adapter<nested2>;

        STATIC_CHECK( std::is_same<cppsort::stable_t<nested1>, sorter>::value );
        STATIC_CHECK( std::is_same<cppsort::stable_t<nested2>, sorter>::value );
        STATIC_CHECK( std::is_same<cppsort::stable_t<nested3>, sorter>::value );
    }

    SECTION( "over stable sorter" )
    {
        // Wrap/nest stable_adapter several times
        using sorter = cppsort::insertion_sorter;
        using nested1 = cppsort::stable_adapter<sorter>;
        using nested2 = cppsort::stable_adapter<nested1>;
        using nested3 = cppsort::stable_adapter<nested2>;

        STATIC_CHECK( std::is_same<cppsort::stable_t<nested1>, sorter>::value );
        STATIC_CHECK( std::is_same<cppsort::stable_t<nested2>, sorter>::value );
        STATIC_CHECK( std::is_same<cppsort::stable_t<nested3>, sorter>::value );
    }
}

TEST_CASE( "stable_t<hybrid_adapter>", "[stable_adapter][hybrid_adapter]" )
{
    using sorter_t = cppsort::hybrid_adapter<
        cppsort::poplar_sorter,
        cppsort::quick_merge_sorter
    >;
    using stable_sorter_t = cppsort::stable_t<sorter_t>;

    sorter_t sorter;
    stable_sorter_t stable_sorter(std::move(sorter));

    CHECK( true );
}
