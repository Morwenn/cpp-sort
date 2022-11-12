/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <string>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <testing-tools/distributions.h>
#include <testing-tools/no_post_iterator.h>
#include <testing-tools/old_default_sorter.h>
#include <testing-tools/random.h>

TEMPLATE_TEST_CASE( "test most sorters with no_post_iterator", "[sorters]",
                    old_default_sorter,
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::d_ary_heap_sorter<6>,
                    cppsort::counting_sorter,
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
                    cppsort::ska_sorter,
                    cppsort::slab_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<cppsort::utility::fixed_buffer<0>> )
{
    std::vector<int> collection;
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 310, -56);

    // Iterators with no post-increment and no post-decrement
    auto first = make_no_post_iterator(collection.begin());
    auto last = make_no_post_iterator(collection.end());

    TestType sorter;
    sorter(first, last);
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}

TEMPLATE_TEST_CASE( "test type-specific sorters with no_post_iterator further",
                    "[sorters][ska_sorter][spread_sorter]",
                    cppsort::ska_sorter,
                    cppsort::spread_sorter )
{
    TestType sorter;
    auto distribution = dist::shuffled{};

    std::vector<float> collection_float;
    distribution.call<float>(std::back_inserter(collection_float), 310, -56);

    // Iterators with no post-increment and no post-decrement
    auto first_float = make_no_post_iterator(collection_float.begin());
    auto last_float = make_no_post_iterator(collection_float.end());

    sorter(first_float, last_float);
    CHECK( std::is_sorted(collection_float.begin(), collection_float.end()) );

    std::vector<double> collection_double;
    distribution.call<double>(std::back_inserter(collection_double), 310, -56);

    // Iterators with no post-increment and no post-decrement
    auto first_double = make_no_post_iterator(collection_double.begin());
    auto last_double = make_no_post_iterator(collection_double.end());

    sorter(first_double, last_double);
    CHECK( std::is_sorted(collection_double.begin(), collection_double.end()) );

    std::vector<std::string> collection_str;
    for (long i = 56 ; i < 366 ; ++i) {
        collection_str.emplace_back(std::to_string(i));
    }
    std::shuffle(collection_str.begin(), collection_str.end(), hasard::engine());

    // Iterators with no post-increment and no post-decrement
    auto first_str = make_no_post_iterator(collection_str.begin());
    auto last_str = make_no_post_iterator(collection_str.end());

    sorter(first_str, last_str);
    CHECK( std::is_sorted(collection_str.begin(), collection_str.end()) );
}
