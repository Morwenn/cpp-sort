/*
 * Copyright (c) 2019-2024 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <type_traits>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <testing-tools/distributions.h>

namespace
{
    template<typename IteratorCategory>
    struct stateful_sorter_impl
    {
        int dummy = 0;

        stateful_sorter_impl() = default;

        explicit constexpr stateful_sorter_impl(int a):
            dummy(a)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename = std::enable_if_t<
                not cppsort::is_projection_iterator_v<Compare, Iterator>
            >
        >
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> int
        {
            cppsort::quick_merge_sort(std::move(first), std::move(last), std::move(compare));
            if (std::is_same<IteratorCategory, std::forward_iterator_tag>::value) {
                return 1;
            }
            if (std::is_same<IteratorCategory, std::bidirectional_iterator_tag>::value) {
                return 2;
            }
            return 3;
        }

        using iterator_category = IteratorCategory;
        using is_always_stable = std::false_type;
    };

    template<typename IteratorCategory=std::forward_iterator_tag>
    struct stateful_sorter:
        cppsort::sorter_facade<stateful_sorter_impl<IteratorCategory>>
    {
        stateful_sorter() = default;
        stateful_sorter(int a):
            cppsort::sorter_facade<stateful_sorter_impl<IteratorCategory>>(a)
        {}
    };
}

TEST_CASE( "test stateful sorters with every adapter",
           "[adapters][adapter_storage]" )
{
    auto distribution = dist::shuffled{};

    // Collections to sort
    std::vector<short int> collection; collection.reserve(15);
    distribution(std::back_inserter(collection), 15, 0);
    std::list<short int> li;
    distribution(std::back_inserter(li), 15, 0);
    std::forward_list<short int> fli;
    distribution(std::front_inserter(fli), 15, 0);

    SECTION( "container_aware_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::container_aware_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "counting_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::counting_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "hybrid_adapter" )
    {
        stateful_sorter<std::forward_iterator_tag> sorter1(41);
        stateful_sorter<std::bidirectional_iterator_tag> sorter2(42);
        stateful_sorter<std::random_access_iterator_tag> sorter3(43);
        cppsort::hybrid_adapter<
            stateful_sorter<std::forward_iterator_tag>,
            stateful_sorter<std::bidirectional_iterator_tag>,
            stateful_sorter<std::random_access_iterator_tag>
        > sort_it(sorter1, sorter2, sorter3);

        int res1 = sort_it(fli, std::greater<>{});
        CHECK( res1 == 1 );
        CHECK( std::is_sorted(fli.begin(), fli.end(), std::greater<>{}) );

        int res2 = sort_it(li, std::greater<>{});
        CHECK( res2 == 2 );
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );

        int res3 = sort_it(collection, std::greater<>{});
        CHECK( res3 == 3 );
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "indirect_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::indirect_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "out_of_place_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::out_of_place_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );

        sort_it(fli, std::greater<>{});
        CHECK( std::is_sorted(fli.begin(), fli.end(), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::schwartz_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::self_sort_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        stateful_sorter<> sorter(42);
        auto sort_it = cppsort::stable_adapter<
            cppsort::self_sort_adapter<stateful_sorter<>>
        >(cppsort::self_sort_adapter<stateful_sorter<>>(sorter));

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::stable_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }

    SECTION( "verge_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::verge_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(collection.begin(), collection.end(), std::greater<>{}) );
    }
}
