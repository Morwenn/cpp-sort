/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2016 Morwenn
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
#include <type_traits>
#include <catch.hpp>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/sort.h>

namespace
{
    enum struct kind
    {
        sort,
        stable_sort,
        sorter
    };

    struct container_none
    {
        struct iterator {};

        auto begin() -> iterator { return {}; }
        auto end() -> iterator { return {}; }
        auto begin() const -> iterator { return {}; }
        auto end() const -> iterator { return {}; }
    };

    struct container_sort:
        virtual container_none
    {
        auto sort()
            -> kind
        {
            return kind::sort;
        }
    };

    struct container_stable_sort:
        virtual container_none
    {
        auto stable_sort()
            -> kind
        {
            return kind::stable_sort;
        }
    };

    struct container_both:
        container_sort,
        container_stable_sort
    {};

    struct dumb_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> kind
        {
            return kind::sorter;
        }

        using is_always_stable = std::true_type;
    };

    struct dumb_sorter:
        cppsort::sorter_facade<dumb_sorter_impl>
    {};
}

TEST_CASE( "self_sort_adapter and usual scenarios",
           "[self_sort_adapter]" )
{
    cppsort::self_sort_adapter<dumb_sorter> sorter;

    SECTION( "container with no sort method" )
    {
        container_none container;
        auto res = sorter(container);
        CHECK( res == kind::sorter );
    }

    SECTION( "container with a sort method" )
    {
        container_sort container;
        auto res = sorter(container);
        CHECK( res == kind::sort );
    }

    SECTION( "container with a stable_sort method" )
    {
        container_stable_sort container;
        auto res = sorter(container);
        CHECK( res == kind::stable_sort );
    }

    SECTION( "container with both a sort and a stable_sort method" )
    {
        container_both container;
        auto res = sorter(container);
        CHECK( res == kind::sort );
    }

    SECTION( "iterators" )
    {
        container_both container;
        auto res = sorter(container.begin(), container.end());
        CHECK( res == kind::sorter );
    }
}

TEST_CASE( "stable_adapter<self_sort_adapter> tests",
           "[self_sort_adapter][stable_adapter]" )
{
    cppsort::stable_adapter<
        cppsort::self_sort_adapter<dumb_sorter>
    > sorter;

    SECTION( "container with no sort method" )
    {
        container_none container;
        auto res = sorter(container);
        CHECK( res == kind::sorter );
    }

    SECTION( "container with a sort method" )
    {
        container_sort container;
        auto res = sorter(container);
        CHECK( res == kind::sorter );
    }

    SECTION( "container with a stable_sort method" )
    {
        container_stable_sort container;
        auto res = sorter(container);
        CHECK( res == kind::stable_sort );
    }

    SECTION( "container with both a sort and a stable_sort method" )
    {
        container_both container;
        auto res = sorter(container);
        CHECK( res == kind::stable_sort );
    }

    SECTION( "iterators" )
    {
        container_both container;
        auto res = sorter(container.begin(), container.end());
        CHECK( res == kind::sorter );
    }
}

TEST_CASE( "stability of self_sort_adapter",
           "[self_sort_adapter][is_stable]" )
{
    using sorter = cppsort::self_sort_adapter<
        dumb_sorter
    >;

    SECTION( "is_always_stable" )
    {
        CHECK( cppsort::is_always_stable<dumb_sorter>::value );
        CHECK( not cppsort::is_always_stable<sorter>::value );
    }

    SECTION( "is_stable" )
    {
        CHECK( cppsort::is_stable<sorter(container_none&)>::value );
        CHECK( not cppsort::is_stable<sorter(container_sort&)>::value );
        CHECK( cppsort::is_stable<sorter(container_stable_sort&)>::value );
        CHECK( not cppsort::is_stable<sorter(container_both&)>::value );
        CHECK( cppsort::is_stable<sorter(container_both::iterator, container_both::iterator)>::value );
    }
}
