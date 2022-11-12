/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <list>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/algorithm.h>
#include <testing-tools/distributions.h>
#include <testing-tools/wrapper.h>

using wrapper = generic_stable_wrapper<int>;

// Those tests do not prove that the algorithms sort stably,
// but are meant to sometimes identify cases where they don't
//
// In order to achieve that, each element is associated to
// its original position in the collection to sort, then the
// collection is sorted accorded to its "value" and we check
// that the collection is indeed sorted, but also that
// the original position of elements with equivalent values
// are also in ascending order

TEMPLATE_TEST_CASE( "every random-access sorter with stable_adapter", "[stable_adapter]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<3>,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::heap_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_insertion_sorter,
                    cppsort::merge_sorter,
                    cppsort::pdq_sorter,
                    cppsort::poplar_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    cppsort::stable_t<TestType> sorter;
    std::vector<wrapper> collection(412);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    SECTION( "shuffled_16_values" )
    {
        auto distribution = dist::shuffled_16_values{};
        distribution(collection.begin(), collection.size());
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "descending_plateau" )
    {
        auto distribution = dist::descending_plateau{};
        distribution(collection.begin(), collection.size());
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}

TEMPLATE_TEST_CASE( "every bidirectional sorter with stable_adapter", "[stable_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::insertion_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter,
                    cppsort::slab_sorter,
                    cppsort::verge_sorter )
{
    cppsort::stable_t<TestType> sorter;
    std::list<wrapper> collection(412);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    SECTION( "shuffled_16_values" )
    {
        auto distribution = dist::shuffled_16_values{};
        distribution(collection.begin(), collection.size());
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "descending_plateau" )
    {
        auto distribution = dist::descending_plateau{};
        distribution(collection.begin(), collection.size());
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}

TEMPLATE_TEST_CASE( "every forward sorter with with stable_adapter", "[stable_adapter]",
                    cppsort::cartesian_tree_sorter,
                    cppsort::mel_sorter,
                    cppsort::merge_sorter,
                    cppsort::quick_merge_sorter,
                    cppsort::quick_sorter,
                    cppsort::selection_sorter )
{
    cppsort::stable_t<TestType> sorter;
    const int size = 412;
    std::forward_list<wrapper> collection(size);
    helpers::iota(collection.begin(), collection.end(), 0, &wrapper::order);

    SECTION( "shuffled_16_values" )
    {
        auto distribution = dist::shuffled_16_values{};
        distribution(collection.begin(), size);
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }

    SECTION( "descending_plateau" )
    {
        auto distribution = dist::descending_plateau{};
        distribution(collection.begin(), size);
        sorter(collection, &wrapper::value);
        CHECK( std::is_sorted(collection.begin(), collection.end()) );
    }
}
