/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 Morwenn
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <algorithm>
#include <forward_list>
#include <functional>
#include <iterator>
#include <list>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/poplar_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include "../distributions.h"

TEST_CASE( "function pointer test for every adapter",
           "[adapters][function_pointer]" )
{
    // Collection to sort
    std::vector<int> collection; collection.reserve(65);
    auto distribution = dist::shuffled{};
    distribution(std::back_inserter(collection), 65, 0);

    SECTION( "container_aware_adapter" )
    {
        using sorter = cppsort::container_aware_adapter<
            cppsort::selection_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};
        void(*sort_it2)(std::list<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "counting_adapter" )
    {
        using sorter = cppsort::counting_adapter<
            cppsort::selection_sorter
        >;
        std::size_t(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        std::size_t res = sort_it(collection, std::greater<>{});
        CHECK( res == 2080 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::merge_sorter,
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "indirect_adapter" )
    {
        using sorter = cppsort::indirect_adapter<
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter" )
    {
        using sorter = cppsort::schwartz_adapter<
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        using sorter = cppsort::self_sort_adapter<
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};
        void(*sort_it2)(std::list<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::self_sort_adapter<cppsort::poplar_sorter>
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};
        void(*sort_it2)(std::list<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        std::list<int> li;
        distribution(std::back_inserter(li), 65, 0);

        sort_it2(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "small_array_adapter" )
    {
        using namespace cppsort;

        using sorter2 = small_array_adapter<low_moves_sorter>;
        using sorter1 = small_array_adapter<low_comparisons_sorter>;
        using sorter3 = small_array_adapter<sorting_network_sorter>;
        void(*sort_it1)(std::array<int, 6>&, std::greater<>) = sorter2{};
        void(*sort_it2)(std::array<int, 6>&, std::greater<>) = sorter1{};
        void(*sort_it3)(std::array<int, 6>&, std::greater<>) = sorter3{};

        std::array<int, 6> arr = {{ 4, 3, 2, 5, 6, 1 }};

        auto to_sort = arr;
        sort_it1(to_sort, std::greater<>{});
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort), std::greater<>{}) );

        to_sort = arr;
        sort_it2(to_sort, std::greater<>{});
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort), std::greater<>{}) );

        to_sort = arr;
        sort_it3(to_sort, std::greater<>{});
        CHECK( std::is_sorted(std::begin(to_sort), std::end(to_sort), std::greater<>{}) );
    }

    SECTION( "stable_adapter" )
    {
        using sorter = cppsort::stable_adapter<
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "verge_adapter" )
    {
        using sorter = cppsort::verge_adapter<
            cppsort::poplar_sorter
        >;
        void(*sort_it)(std::vector<int>&, std::greater<>) = sorter{};

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }
}
