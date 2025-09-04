/*
 * Copyright (c) 2016-2025 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <forward_list>
#include <random>
#include <type_traits>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <rapidcheck.h>
#include <rapidcheck/catch.h>
#include <cpp-sort/probes/dis.h>
#include <cpp-sort/utility/size.h>
#include <testing-tools/internal_compare.h>
#include <testing-tools/random.h>

TEST_CASE( "measure of disorder: dis", "[probe][dis]" )
{
    using cppsort::probe::dis;

    SECTION( "simple tests" )
    {
        {
            const std::forward_list<int> li = { 47, 53, 46, 41, 59, 81, 74, 97, 100, 45 };
            CHECK( dis(li) == 9 );
            CHECK( dis(li.begin(), li.end()) == 9 );

            const std::vector<internal_compare<int>> tricky(li.begin(), li.end());
            CHECK( dis(tricky, &internal_compare<int>::compare_to) == 9 );
        }
        {
            const std::forward_list<int> li = { 48, 43, 96, 44, 42, 34, 42, 57, 68, 69 };
            CHECK( dis(li) == 7 );
            CHECK( dis(li.begin(), li.end()) == 7 );

            const std::vector<internal_compare<int>> tricky(li.begin(), li.end());
            CHECK( dis(tricky, &internal_compare<int>::compare_to) == 7 );
        }
    }

    SECTION( "roughly sorting test" )
    {
        // Example from *Roughly Sorting: Sequential and Parallel Approach*
        // by T. Altman and Y. Igarashi

        const std::forward_list<int> li = {
            2, 3, 5, 1, 4, 2, 6,
            8, 7, 9, 8, 11, 6, 13,
            12, 16, 15, 17, 18,
            20, 18, 19, 21, 19
        };
        CHECK( dis(li) == 5 );
    }

    SECTION( "upper bound" )
    {
        // The upper bound should correspond to the size of
        // the input sequence minus one

        const std::forward_list<int> li = { 10, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };
        auto max_n = dis.max_for_size(cppsort::utility::size(li));
        CHECK( max_n == 10 );
        CHECK( dis(li) == max_n );
        CHECK( dis(li.begin(), li.end()) == max_n );
    }

    // Sorting and Measures of Disorder
    // by Vladimir Estivill-Castro

    rc::prop("Dis(XY) = max{Dis(X), Dis(Y)} if X ≤ Y", [](std::vector<int> sequence) {
        using diff_t = std::vector<int>::difference_type;
        using param_t = std::uniform_int_distribution<diff_t>::param_type;

        // Split the sequence into two consecutive subsequences X and Y
        auto size = static_cast<diff_t>(sequence.size());
        std::uniform_int_distribution<diff_t> dist;
        auto x_begin = sequence.begin();
        auto y_begin = x_begin + dist(hasard::engine(), param_t{0, size});
        std::nth_element(x_begin, y_begin, sequence.end());

        using cppsort::probe::dis;
        return dis(sequence) == (std::max)(dis(x_begin, y_begin), dis(y_begin, sequence.end()));
    });
}
