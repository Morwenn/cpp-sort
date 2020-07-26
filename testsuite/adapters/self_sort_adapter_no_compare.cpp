/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2020 Morwenn
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
#include <functional>
#include <iterator>
#include <list>
#include <utility>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/sorters/verge_sorter.h>
#include <cpp-sort/sorter_facade.h>
#include <testing-tools/distributions.h>

namespace
{
    enum struct sorter_type
    {
        dummy_sorter,
        self_sortable
    };

    struct dummy_sorter_impl
    {
        template<typename... Args>
        auto operator()(Args&&... args) const
            -> sorter_type
        {
            cppsort::verge_sorter{}(std::forward<Args>(args)...);
            return sorter_type::dummy_sorter;
        }
    };

    struct dummy_sorter:
        cppsort::sorter_facade<dummy_sorter_impl>
    {};

    // This class can sort itself but its sort method
    // does not accept a comparator
    struct non_comparison_self_sortable
    {
        std::list<int> elements;

        template<typename... Args>
        explicit non_comparison_self_sortable(Args... args):
            elements(args...)
        {}

        auto begin() { return elements.begin(); }
        auto end() { return elements.end(); }
        auto begin() const { return elements.begin(); }
        auto end() const { return elements.end(); }

        // No comparison function
        auto sort()
            -> sorter_type
        {
            elements.sort();
            return sorter_type::self_sortable;
        }
    };
}

TEST_CASE( "self-sortable object without comparison",
           "[self_sort_adapter][no_compare]" )
{
    // Collection to sort
    auto size = 80;
    non_comparison_self_sortable collection(size);

    SECTION( "without a comparator" )
    {
        // Make sure the sort method is used when no
        // comparator is given

        cppsort::self_sort_adapter<
            dummy_sorter
        > sorter;

        // Fill the collection
        std::vector<int> tmp; tmp.reserve(size);
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(tmp), size, 0);
        std::copy(std::begin(tmp), std::end(tmp), std::begin(collection));

        // Sort and check it's sorted
        auto res = sorter(collection);
        CHECK( res == sorter_type::self_sortable );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }

    SECTION( "with a comparator" )
    {
        // Make sure the fallback sorter is used when a
        // comparator is given

        cppsort::self_sort_adapter<
            dummy_sorter
        > sorter;

        // Fill the collection
        std::vector<int> tmp; tmp.reserve(size);
        auto distribution = dist::shuffled{};
        distribution(std::back_inserter(tmp), size, 0);
        std::copy(std::begin(tmp), std::end(tmp), std::begin(collection));

        // Sort and check it's sorted
        auto res = sorter(collection, std::less<>{});
        CHECK( res == sorter_type::dummy_sorter );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection)) );
    }
}
