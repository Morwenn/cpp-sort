/*
 * Copyright (c) 2019-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <iterator>
#include <string>
#include <utility>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/utility/buffer.h>
#include <cpp-sort/utility/functional.h>
#include <testing-tools/distributions.h>
#include <testing-tools/random.h>

namespace
{
    struct shuffled_string:
        dist::distribution<shuffled_string>
    {
        template<typename OutputIterator, typename T=long long int>
        auto operator()(OutputIterator out, long long int size, T start=T(0)) const
            -> void
        {
            std::vector<std::string> vec;
            vec.reserve(size);

            T end = start + size;
            for (auto i = start ; i < end ; ++i) {
                auto s = std::to_string(i);
                vec.push_back(std::string(100 - s.size(), '0') + std::move(s));
            }
            std::shuffle(std::begin(vec), std::end(vec), hasard::engine());
            std::move(std::begin(vec), std::end(vec), out);
        }
    };
}

TEMPLATE_TEST_CASE( "test every sorter with long std::string", "[sorters]",
                    cppsort::cartesian_tree_sorter,
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
                    cppsort::std_sorter,
                    cppsort::tim_sorter,
                    cppsort::verge_sorter,
                    cppsort::wiki_sorter<>,
                    cppsort::wiki_sorter<
                        cppsort::utility::dynamic_buffer<cppsort::utility::half>
                    > )
{
    // This test primarily exists to detect self-move issues in the library:
    // long std::string instances actually self-destruct on self-move and can
    // thus give incorrect results or even destroy the values of the collection
    //
    // Instead of checking that the result is sorted, we check that it is equal
    // to the same array is sorted exactly the same way with std::sort, which
    // is actually a stronger check and even requires a total order for the
    // comparison function

    std::vector<std::string> collection;
    collection.reserve(491);
    auto distribution = shuffled_string{};
    distribution(std::back_inserter(collection), 491, -125);

    auto copy = collection;
    std::sort(std::begin(copy), std::end(copy));

    TestType sorter;
    sorter(collection);
    CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    CHECK( bool(collection == copy) );
}
