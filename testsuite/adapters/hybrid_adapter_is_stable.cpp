/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2018 Morwenn
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
#include <array>
#include <forward_list>
#include <iterator>
#include <list>
#include <utility>
#include <vector>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/small_array_adapter.h>
#include <cpp-sort/fixed/sorting_network_sorter.h>
#include <cpp-sort/sorter_traits.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include <cpp-sort/sorters/pdq_sorter.h>
#include <cpp-sort/sorters/selection_sorter.h>
#include <catch2/catch.hpp>

TEST_CASE( "hybrid_adapter stability checks",
           "[hybrid_adapter][is_stable]" )
{
    SECTION( "simple sorters" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::selection_sorter,
            cppsort::rebind_iterator_category<
                cppsort::merge_sorter,
                std::bidirectional_iterator_tag
            >,
            cppsort::pdq_sorter
        >;

        CHECK(( not cppsort::is_stable<sorter(std::vector<int>&)>::value ));
        CHECK(( not cppsort::is_stable<sorter(std::vector<int>::iterator,
                                              std::vector<int>::iterator)>::value ));

        CHECK(( cppsort::is_stable<sorter(std::list<int>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::list<int>::iterator,
                                          std::list<int>::iterator)>::value ));

        CHECK(( not cppsort::is_stable<sorter(std::forward_list<int>&)>::value ));
        CHECK(( not cppsort::is_stable<sorter(std::forward_list<int>::iterator,
                                              std::forward_list<int>::iterator)>::value ));
    }

    SECTION( "nested hybrid_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::selection_sorter,
            cppsort::hybrid_adapter<
                cppsort::rebind_iterator_category<
                    cppsort::merge_sorter,
                    std::bidirectional_iterator_tag
                >
            >,
            cppsort::pdq_sorter
        >;

        CHECK(( not cppsort::is_stable<sorter(std::vector<int>&)>::value ));
        CHECK(( not cppsort::is_stable<sorter(std::vector<int>::iterator,
                                              std::vector<int>::iterator)>::value ));

        CHECK(( cppsort::is_stable<sorter(std::list<int>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::list<int>::iterator,
                                          std::list<int>::iterator)>::value ));

        CHECK(( not cppsort::is_stable<sorter(std::forward_list<int>&)>::value ));
        CHECK(( not cppsort::is_stable<sorter(std::forward_list<int>::iterator,
                                              std::forward_list<int>::iterator)>::value ));
    }

    SECTION( "with small_array_adapter" )
    {
        using sorter = cppsort::hybrid_adapter<
            cppsort::small_array_adapter<
                cppsort::sorting_network_sorter,
                std::make_index_sequence<14u>
            >,
            cppsort::merge_sorter
        >;

        CHECK(( cppsort::is_stable<sorter(std::vector<int>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::vector<int>::iterator,
                                          std::vector<int>::iterator)>::value ));

        CHECK(( cppsort::is_stable<sorter(std::list<int>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::list<int>::iterator,
                                          std::list<int>::iterator)>::value ));

        CHECK(( cppsort::is_stable<sorter(std::forward_list<int>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::forward_list<int>::iterator,
                                          std::forward_list<int>::iterator)>::value ));

        CHECK(( not cppsort::is_stable<sorter(std::array<int, 5>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::array<int, 5>::iterator,
                                          std::array<int, 5>::iterator)>::value ));

        CHECK(( cppsort::is_stable<sorter(std::array<int, 16>&)>::value ));
        CHECK(( cppsort::is_stable<sorter(std::array<int, 16>::iterator,
                                          std::array<int, 16>::iterator)>::value ));

        CHECK(( not cppsort::is_stable<sorter(int(&)[8])>::value ));
        CHECK(( cppsort::is_stable<sorter(int(&)[20])>::value ));
    }
}
