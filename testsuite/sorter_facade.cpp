/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Morwenn
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
#include <iterator>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/sort.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorter_traits.h>

namespace
{
    struct comparison_sorter_impl
    {
        // Pair of iterators overload
        template<typename Iterator, typename Compare=std::less<>>
        auto operator()(Iterator, Iterator, Compare={}) const
            -> bool
        {
            return true;
        }

        // Iterable overload
        template<typename Iterable, typename Compare=std::less<>>
        auto operator()(Iterable&, Compare={}) const
            -> bool
        {
            return false;
        }
    };

    struct comparison_sorter:
        cppsort::sorter_facade<comparison_sorter_impl>
    {};
}

TEST_CASE( "miscellaneous sorter_facade checks",
           "[sorter_facade][compare]" )
{
    // Miscellaneous sorter_facade checks to make sure
    // that no operator() ambiguity is introduced

    // Vector to "sort"
    std::vector<int> vec(3);

    SECTION( "conditional iterable operator() overload" )
    {
        CHECK( not cppsort::sort(vec, comparison_sorter{}, std::less<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, std::less<>{}) );

        CHECK( not cppsort::sort(vec, comparison_sorter{}, std::greater<>{}) );
        CHECK( cppsort::sort(std::begin(vec), std::end(vec), comparison_sorter{}, std::greater<>{}) );
    }
}
