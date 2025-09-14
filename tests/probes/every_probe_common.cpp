/*
 * Copyright (c) 2021-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <numeric>
#include <random>
#include <type_traits>
#include <vector>
#include <catch2/catch_template_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes.h>
#include "testing-tools/random.h"

//
// Test common properties of measures of presortedness
//

TEMPLATE_TEST_CASE( "test every probe with all_equal distribution", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
    // Ensure that all measures of disorder return 0 when
    // given a collection where all elements are equal

    const std::vector<int> collection(50, 5);
    std::decay_t<TestType> measure;
    auto disorder = measure(collection);
    CHECK( disorder == 0 );
}

TEMPLATE_TEST_CASE( "test every probe with a sorted collection", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
    // Ensure that all measures of disorder return 0 when
    // given a collection where all elements are sorted

    std::vector<int> collection(50);
    std::iota(collection.begin(), collection.end(), 0);
    std::decay_t<TestType> measure;
    auto disorder = measure(collection);
    CHECK( disorder == 0 );
}

TEMPLATE_TEST_CASE( "test every probe with a 0 or 1 element", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // First property formalized by Mannila
    // Ensure that all measures of disorder return 0 when
    // given a collection with 0 or 1 element

    std::decay_t<TestType> measure;

    SECTION( "empty collection" )
    {
        const std::vector<int> collection;
        auto disorder = measure(collection);
        CHECK( disorder == 0 );
        CHECK( measure.max_for_size(0) == 0 );
    }

    SECTION( "one-element collection" )
    {
        const std::vector<int> collection = { 42 };
        auto disorder = measure(collection);
        CHECK( disorder == 0 );
        CHECK( measure.max_for_size(0) == 0 );
    }
}

TEMPLATE_TEST_CASE( "test order isomorphism for every probe", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Second property formalized by Mannila
    // Ensure that when the relative order of elements of two sequences
    // is the same, then the measure of disorder returns the same
    // result

    rc::prop("order isomorphism", [](std::vector<int> sequence1) {
        std::decay_t<TestType> measure;

        for (int& elem: sequence1) {
            elem /= 2;
        }

        auto sequence2 = sequence1;
        for (int& elem: sequence2) {
            elem *= 2;
        }

        return measure(sequence1) == measure(sequence2);
    });
}

namespace
{
    auto get_random_subsequence(std::vector<int> sequence)
        -> std::vector<int>
    {
        std::uniform_int_distribution<int> uni(0, sequence.size());

        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;
        std::uniform_int_distribution<diff_t> dist;

        diff_t size = sequence.size();
        auto number_of_elements_to_remove = dist(hasard::engine(), param_t{0, size});
        for (; number_of_elements_to_remove > 0; --number_of_elements_to_remove) {
            auto idx = dist(hasard::engine(), param_t{0, size - 1});
            sequence.erase(sequence.begin() + idx);
            --size;
        }

        return sequence;
    }
}

TEMPLATE_TEST_CASE( "test M(subsequence(X)) <= M(X) for most probes M", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Third property formalized by Mannila
    // Ensure that the disorder that exists in a subsequence is no
    // greater than the disorder that exists in the whole sequence

    rc::prop("M(subsequence(X)) ≤ M(X)", [](const std::vector<int>& sequence) {
        auto subsequence = get_random_subsequence(sequence);
        std::decay_t<TestType> measure;
        return measure(subsequence) <= measure(sequence);
    });
}

TEMPLATE_TEST_CASE( "test M(subsequence(X)) <= M(X) for most probes M (unique elements)", "[probe]",
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::block) )
{
    // Same as above, but for probes that aren't well-behaved
    // with regard to equivalent elements

    rc::prop("M(subsequence(X)) ≤ M(X)", []() {
        auto sequence = *rc::gen::unique<std::vector<int>>(rc::gen::arbitrary<int>());
        auto subsequence = get_random_subsequence(sequence);
        std::decay_t<TestType> measure;
        return measure(subsequence) <= measure(sequence);
    });
}

namespace
{
    // Split a sequence into two consequent subsequences X and Y,
    // with all elements of X being not greater than all elements of Y,
    // the split point is chosen at random

    auto split_in_two(std::vector<int>& sequence)
        -> std::vector<int>::iterator
    {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        auto size = static_cast<diff_t>(sequence.size());
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size});
        std::nth_element(x_begin, y_begin, sequence.end());
        return y_begin;
    }
}

TEMPLATE_TEST_CASE( "test M(XY) <= M(X) + M(Y) if X <= Y for most probes M", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::sus) )
{
    // Fourth property formalized by Mannila
    // Ensure that the disorder found in the concatenation of two sequences is not
    // greater than the sum of the individual sequences' disorders when all elements
    // of the second sequence are greater than all elements of the first sequence

    // Note: some measures do not appear here because we test a stronger bound
    //       instead (see the next test)

    rc::prop("M(XY) ≤ M(X) + M(Y) if X ≤ Y", [](std::vector<int> sequence) {
        auto y_begin = split_in_two(sequence);
        std::decay_t<TestType> measure;
        return measure(sequence) <= measure(sequence.begin(), y_begin) + measure(y_begin, sequence.end());
    });
}

TEMPLATE_TEST_CASE( "test M(XY) = M(X) + M(Y) if X <= Y for some probes M", "[probe]",
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear) )
{
    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*
    // It is a stronger bound on Mannila's fourth property that some measures satisfy

    // Note: probe::exc also satisfies this property, but our implementation only
    //       handles it correctly when no equivalent elements exist in the sequence,
    //       so we test it in its own file

    rc::prop("M(XY) = M(X) + M(Y) if X ≤ Y", [](std::vector<int> sequence) {
        auto y_begin = split_in_two(sequence);
        std::decay_t<TestType> measure;
        return measure(sequence) == measure(sequence.begin(), y_begin) + measure(y_begin, sequence.end());
    });
}

TEMPLATE_TEST_CASE( "test M(2, 1, 4, 3, 6, 5, ...) <= |X| * M(2, 1) / 2 for most probes M", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // From *Sorting and Measures of Disorder* by Estivill-Castro:
    // property derived from Mannila's criteria 2 & 4
    // The following probes don't satisfy it: Block, Mono, Osc

    int size = 1000;
    std::vector<int> sequence(size, 0);
    std::iota(sequence.begin(), sequence.end(), 0);
    for (int i = 0; i < size; i += 2) {
        std::swap(sequence[i], sequence[i + 1]);
    }

    int inversion[2] = {2, 1};

    std::decay_t<TestType> measure;
    CHECK( measure(sequence) <= size * measure(inversion) / 2 );
}

TEMPLATE_TEST_CASE( "test M(aX) <= |X| + M(X) for most probes M", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::sus) )
{
    // Fifth property formalized by Mannila
    // The following probes don't satisfy it: Ham, Osc, Spear

    rc::prop("M(⟨a⟩X) ≤ |X| + M(X)", [](const std::vector<int>& sequence) {
        std::decay_t<TestType> measure;

        auto size = static_cast<std::vector<int>::difference_type>(sequence.size());
        if (size <= 2) {
            return true;
        }

        return measure(sequence) <= (size - 1)
             + measure(sequence.begin() + 1, sequence.end());
    });
}

TEMPLATE_TEST_CASE( "test prefix monotonicity", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*
    // The following probes don't satisfy it: Block, Mono, Osc

    // Note: the original paper claims that Osc also satisfies this property,
    // but it fails for X=⟨3, 0⟩ Y=⟨⟩ Z=⟨4, 2⟩

    rc::prop("prefix monotonicity", [](std::vector<int> sequence) {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        auto size = static_cast<diff_t>(sequence.size());
        if (size < 3) {
            return true;
        }

        // Split the sequence into three consequent subsequences X, Y and Z
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size - 1});
        auto z_begin = x_begin + dist(hasard::engine(), param_t{y_begin - x_begin, size - 1});

        // Ensure that all elements of Z are greater than all elements of X and Y
        std::nth_element(x_begin, z_begin, sequence.end());

        std::decay_t<TestType> measure;
        auto disorder_x = measure(x_begin, y_begin);
        auto disorder_y = measure(y_begin, z_begin);
        auto disorder_yz = measure(y_begin, sequence.end());
        auto new_x_begin = std::rotate(x_begin, y_begin, z_begin);
        auto disorder_xz = measure(new_x_begin, sequence.end());

        return disorder_x <= disorder_y
            ? disorder_xz <= disorder_yz
            : disorder_yz <= disorder_xz;
    });
}

TEMPLATE_TEST_CASE( "test monotonicity", "[probe]",
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*
    // The following probes don't satisfy it: Block, Enc, Mono, Osc

    // Note: the original paper claims that MEnc[k,A,D] also satisfies this property,
    // but at the time of writing this comment I ahev no idea what that means

    rc::prop("monotonicity", [](std::vector<int> sequence) {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        auto size = static_cast<diff_t>(sequence.size());
        if (size < 4) {
            return true;
        }

        // Split the sequence into three consequent subsequences W, X and Z
        std::uniform_int_distribution<diff_t> dist;
        auto w_begin = sequence.begin();
        auto x_begin = w_begin + dist(hasard::engine(), param_t{0, size - 1});
        auto z_begin = w_begin + dist(hasard::engine(), param_t{x_begin - w_begin, size - 1});

        // Ensure that all elements of Z are greater than all elements of W and X
        std::nth_element(w_begin, z_begin, sequence.end());
        // Ensure that all elements of W are smaller than all elements of X
        std::nth_element(w_begin, x_begin, z_begin);

        std::decay_t<TestType> measure;
        auto disorder_x = measure(x_begin, z_begin);
        auto disorder_wxz = measure(sequence);

        // Create Y such as W <= Y <= Z by shuffling X
        std::shuffle(x_begin, z_begin, hasard::engine());
        auto disorder_y = measure(x_begin, z_begin);
        auto disorder_wyz = measure(sequence);

        return disorder_x <= disorder_y
            ? disorder_wxz <= disorder_wyz
            : disorder_wyz <= disorder_wxz;
    });
}

TEMPLATE_TEST_CASE( "test that probes never produce more disorder than their theoretical maximum", "[probe]",
                    decltype(cppsort::probe::block),
                    decltype(cppsort::probe::dis),
                    decltype(cppsort::probe::enc),
                    decltype(cppsort::probe::exc),
                    decltype(cppsort::probe::ham),
                    decltype(cppsort::probe::inv),
                    decltype(cppsort::probe::max),
                    decltype(cppsort::probe::mono),
                    decltype(cppsort::probe::osc),
                    decltype(cppsort::probe::rem),
                    decltype(cppsort::probe::runs),
                    decltype(cppsort::probe::spear),
                    decltype(cppsort::probe::sus) )
{
    rc::prop("M(X) ≤ max_for_size(|X|)", [](const std::vector<int>& sequence) {
        using diff_t = std::vector<int>::difference_type;
        std::decay_t<TestType> measure;
        return measure(sequence) <= measure.template max_for_size<diff_t>(sequence.size());
    });
}
