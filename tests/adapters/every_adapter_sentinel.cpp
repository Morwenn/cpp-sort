/*
 * Copyright (c) 2022-2023 Morwenn
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
#include <cpp-sort/sorters/mel_sorter.h>
#include <testing-tools/distributions.h>
#include <testing-tools/end_sentinel.h>

TEST_CASE( "test sentinel support for every adapter",
           "[adapters][adapter_storage]" )
{
    auto distribution = dist::shuffled{};

    // Collections to sort
    std::vector<int> vec;
    distribution(std::back_inserter(vec), 300);
    auto vec_begin = vec.begin();
    auto vec_end = end_sentinel(vec.end());

    std::list<int> li;
    distribution(std::back_inserter(li), 300);
    auto li_begin = li.begin();
    auto li_end = end_sentinel(li.end());

    std::forward_list<int> fli;
    distribution(std::front_inserter(fli), 300);
    auto fli_begin = fli.begin();
    auto fli_end = end_sentinel(fli.end());

    SECTION( "container_aware_adapter" )
    {
        cppsort::container_aware_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        sort_it(li_begin, li_end, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );

        sort_it(fli_begin, fli_end, std::greater<>{});
        CHECK( std::is_sorted(fli.begin(), fli.end(), std::greater<>{}) );
    }

    SECTION( "drop_merge_adapter" )
    {
        cppsort::drop_merge_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "indirect_adapter" )
    {
        cppsort::indirect_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "out_of_place_adapter" )
    {
        cppsort::out_of_place_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        sort_it(li_begin, li_end, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );

        sort_it(fli_begin, fli_end, std::greater<>{});
        CHECK( std::is_sorted(fli.begin(), fli.end(), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter" )
    {
        cppsort::schwartz_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        cppsort::self_sort_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        sort_it(li_begin, li_end, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        auto sort_it = cppsort::stable_adapter(
            cppsort::self_sort_adapter(
                cppsort::mel_sort
            )
        );

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );

        sort_it(li_begin, li_end, std::greater<>{});
        CHECK( std::is_sorted(li.begin(), li.end(), std::greater<>{}) );
    }

    SECTION( "split_adapter" )
    {
        cppsort::split_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "stable_adapter" )
    {
        cppsort::stable_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }

    SECTION( "verge_adapter" )
    {
        cppsort::verge_adapter sort_it(cppsort::mel_sort);

        sort_it(vec_begin, vec_end, std::greater<>{});
        CHECK( std::is_sorted(vec.begin(), vec.end(), std::greater<>{}) );
    }
}
