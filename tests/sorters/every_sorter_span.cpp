/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/span.h>

TEMPLATE_TEST_CASE( "test every sorter with temporary span", "[sorters][span]",
                    cppsort::adaptive_shivers_sorter,
                    cppsort::cartesian_tree_sorter,
                    cppsort::counting_sorter,
                    cppsort::d_ary_heap_sorter<2>,
                    cppsort::default_sorter,
                    cppsort::drop_merge_sorter,
                    cppsort::grail_sorter<>,
                    cppsort::grail_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::sqrt>
                    >,
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
                    cppsort::ska_sorter,
                    cppsort::smooth_sorter,
                    cppsort::spin_sorter,
                    cppsort::split_sorter,
                    cppsort::spread_sorter,
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    // General test to make sure that every sorter compiles fine
    // and is able to sort a temporary span referencing a vector
    // of numbers

    std::vector<long int> collection; collection.reserve(491);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 491, -125);

    TestType sorter;
    sorter(make_span(collection));
    CHECK( std::is_sorted(collection.begin(), collection.end()) );
}
