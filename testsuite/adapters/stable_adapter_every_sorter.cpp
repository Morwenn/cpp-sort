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
#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <random>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/sorters.h>
#include <cpp-sort/stable_sort.h>
#include <cpp-sort/utility/buffer.h>

namespace
{
    struct wrapper
    {
        int value;
        int order;
    };

    auto operator<(const wrapper& lhs, const wrapper& rhs)
        -> bool
    {
        if (lhs.value < rhs.value)
        {
            return true;
        }
        if (rhs.value < lhs.value)
        {
            return false;
        }
        return lhs.order < rhs.order;
    }
}

TEST_CASE( "every sorter with stable adapter",
           "[stable_adapter]" )
{
    std::vector<wrapper> collection(412);
    std::size_t count = 0;
    for (wrapper& wrap: collection)
    {
        wrap.value = count++ % 17;
    }
    std::mt19937 engine(Catch::rngSeed());
    std::shuffle(std::begin(collection), std::end(collection), engine);
    count = 0;
    for (wrapper& wrap: collection)
    {
        wrap.order = count++;
    }

    SECTION( "block_sorter" )
    {
        using sorter = cppsort::block_sorter<cppsort::utility::fixed_buffer<0>>;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "default_sorter" )
    {
        using sorter = cppsort::default_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "drop_merge_sorter" )
    {
        using sorter = cppsort::drop_merge_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "grail_sorter" )
    {
        using sorter = cppsort::grail_sorter<>;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "heap_sorter" )
    {
        using sorter = cppsort::heap_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "insertion_sorter" )
    {
        using sorter = cppsort::insertion_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_insertion_sorter" )
    {
        using sorter = cppsort::merge_insertion_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "merge_sorter" )
    {
        using sorter = cppsort::merge_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "pdq_sorter" )
    {
        using sorter = cppsort::pdq_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "poplar_sorter" )
    {
        using sorter = cppsort::poplar_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_merge_sorter" )
    {
        using sorter = cppsort::quick_merge_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "quick_sorter" )
    {
        using sorter = cppsort::quick_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "selection_sorter" )
    {
        using sorter = cppsort::selection_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "smooth_sorter" )
    {
        using sorter = cppsort::smooth_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "std_sorter" )
    {
        using sorter = cppsort::std_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "tim_sorter" )
    {
        using sorter = cppsort::tim_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "verge_sorter" )
    {
        using sorter = cppsort::verge_sorter;
        cppsort::stable_sort(sorter{}, collection, &wrapper::value);
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
