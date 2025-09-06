/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <forward_list>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/exc.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/distributions.h>
#include <testing-tools/internal_compare.h>

TEST_CASE( "measure of disorder: exc", "[probe][exc]" )
{
    using cppsort::probe::exc;

    SECTION( "simple test" )
    {
        const std::forward_list<int> li = { 74, 59, 62, 23, 86, 69, 18, 52, 77, 68 };
        CHECK( exc(li) == 7 );
        CHECK( exc(li.begin(), li.end()) == 7 );

        const std::forward_list<int> li2 = { 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 };
        CHECK( exc(li2) == 5 );
        CHECK( exc(li2.begin(), li2.end()) == 5 );

        const std::vector<internal_compare<int>> tricky(li.begin(), li.end());
        CHECK( exc(tricky, &internal_compare<int>::compare_to) == 7 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::forward_list<int> li = { 10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        auto max_n = exc.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( exc(li) == max_n );
        CHECK( exc(li.begin(), li.end()) == max_n );
    }

    SECTION( "regression: ascending duplicates" )
    {
        std::vector<int> collection;
        collection.reserve(100);
        auto distribution = dist::ascending_duplicates{};
        distribution(std::back_inserter(collection), 100);

        CHECK( exc(collection) == 0 );
    }

    SECTION( "regression: first and last elements of a cycle compare equal" )
    {
        const std::vector<int> collection = { 0, 0, -1 };
        CHECK( exc(collection) == 1 );
    }

    SECTION( "Example of Exc(subsequence(X)) > Exc(X)" )
    {
        const std::forward_list<int> seq =    { 3, 1, 2, 0 };
        const std::forward_list<int> subseq = { 3, 1, 2 };
        CHECK( exc(seq) == 1 );
        CHECK( exc(subseq) == 2 );
    }

    // Property formalized by Estivill-Castro in *Sorting and Measures of Disorder*,
    // only works when X has distinct values

    rc::prop("Exc(XY) = Exc(X) + Exc(Y) if X ≤ Y", []() {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        auto sequence =  *rc::gen::unique<std::vector<int>>(rc::gen::arbitrary<int>());

        // Split the sequence into two consequent subsequences X and Y
        auto size = static_cast<diff_t>(sequence.size());
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size});
        std::nth_element(x_begin, y_begin, sequence.end());

        using cppsort::probe::exc;
        return exc(sequence) == exc(x_begin, y_begin) + exc(y_begin, sequence.end());
    });
}
