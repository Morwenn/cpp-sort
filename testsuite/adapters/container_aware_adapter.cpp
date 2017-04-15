/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016-2017 Morwenn
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
#include <functional>
#include <list>
#include <type_traits>
#include <catch.hpp>
#include <cpp-sort/adapters/container_aware_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorters/merge_sorter.h>
#include "../algorithm.h"

namespace foobar
{
    template<typename T>
    struct cool_list:
        std::list<T>
    {
        using std::list<T>::list;
    };

    template<
        typename T,
        typename Compare,
        typename = std::enable_if_t<
            not cppsort::is_projection<Compare, T>::value
        >
    >
    auto sort(cppsort::merge_sorter, cool_list<T>&, Compare)
        -> bool
    {
        return true;
    }
}

TEST_CASE( "basic tests with container_aware_adapter",
           "[container_aware_adapter]" )
{
    using sorter = cppsort::container_aware_adapter<
        cppsort::merge_sorter
    >;

    // Cool list to "sort"
    foobar::cool_list<int> collection;

    SECTION( "with comparison" )
    {
        CHECK( sorter{}(collection, std::greater<>{}) );
        CHECK( cppsort::sort(sorter{}, collection, std::greater<>{}) );
        CHECK( not cppsort::is_stable<sorter(foobar::cool_list<int>&, std::greater<>)>::value );
    }

    SECTION( "with projection" )
    {
        CHECK( sorter{}(collection, std::negate<>{}) );
        CHECK( cppsort::sort(sorter{}, collection, std::negate<>{}) );
        CHECK( not cppsort::is_stable<sorter(foobar::cool_list<int>&, std::negate<>)>::value );
    }

    SECTION( "with automagic comparison-projection" )
    {
        CHECK( sorter{}(collection, std::greater<>{}, std::negate<>{}) );
        CHECK( cppsort::sort(sorter{}, collection, std::greater<>{}, std::negate<>{}) );
        CHECK( not cppsort::is_stable<sorter(foobar::cool_list<int>&, std::greater<>, std::negate<>)>::value );
    }

    SECTION( "more about stability" )
    {
        CHECK( cppsort::is_stable<sorter(std::list<int>&)>::value );
        CHECK( cppsort::is_stable<sorter(std::list<int>::iterator, std::list<int>::iterator)>::value );
        CHECK( cppsort::is_stable<sorter(foobar::cool_list<int>::iterator,
                                         foobar::cool_list<int>::iterator)>::value );
    }
}
