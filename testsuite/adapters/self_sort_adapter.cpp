/*
 * Copyright (c) 2015-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <type_traits>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/adapters/hybrid_adapter.h>
#include <cpp-sort/adapters/self_sort_adapter.h>
#include <cpp-sort/adapters/stable_adapter.h>
#include <cpp-sort/sorter_facade.h>
#include <cpp-sort/sorters/selection_sorter.h>

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
        using iterator = int*;

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

    struct dumb_unstable_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> kind
        {
            return kind::sorter;
        }

        using is_always_stable = std::false_type;
    };

    struct dumb_unstable_sorter:
        cppsort::sorter_facade<dumb_unstable_sorter_impl>
    {};

    struct dumb_stable_sorter_impl
    {
        template<typename Iterator>
        auto operator()(Iterator, Iterator) const
            -> kind
        {
            return kind::sorter;
        }

        using is_always_stable = std::true_type;
    };

    struct dumb_stable_sorter:
        cppsort::sorter_facade<dumb_stable_sorter_impl>
    {};
}

TEST_CASE( "self_sort_adapter and usual scenarios",
           "[self_sort_adapter]" )
{
    cppsort::self_sort_adapter<dumb_stable_sorter> sorter;

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
        cppsort::self_sort_adapter<dumb_stable_sorter>
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
    using adapted_unstable_sorter = cppsort::self_sort_adapter<
        dumb_unstable_sorter
    >;

    using adapted_stable_sorter = cppsort::self_sort_adapter<
        dumb_stable_sorter
    >;

    SECTION( "is_always_stable" )
    {
        STATIC_CHECK( not cppsort::is_always_stable<dumb_unstable_sorter>::value );
        STATIC_CHECK( not cppsort::is_always_stable<adapted_unstable_sorter>::value );

        STATIC_CHECK( cppsort::is_always_stable<dumb_stable_sorter>::value );
        STATIC_CHECK( not cppsort::is_always_stable<adapted_stable_sorter>::value );
    }

    SECTION( "is_stable" )
    {
        STATIC_CHECK( not cppsort::is_stable<adapted_unstable_sorter(container_none&)>::value );
        STATIC_CHECK( not cppsort::is_stable<adapted_unstable_sorter(container_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_stable_sort&)>::value );
        STATIC_CHECK( not cppsort::is_stable<adapted_unstable_sorter(container_both&)>::value );
        STATIC_CHECK( not cppsort::is_stable<adapted_unstable_sorter(container_both::iterator,
                                                                     container_both::iterator)>::value );

        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_none&)>::value );
        STATIC_CHECK( not cppsort::is_stable<adapted_stable_sorter(container_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_stable_sort&)>::value );
        STATIC_CHECK( not cppsort::is_stable<adapted_stable_sorter(container_both&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_both::iterator,
                                                               container_both::iterator)>::value );
    }
}

TEST_CASE( "stability of stable_adapter<self_sort_adapter>",
           "[self_sort_adapter][stable_adapter][is_stable]" )
{
    using adapted_unstable_sorter = cppsort::stable_adapter<
        cppsort::self_sort_adapter<dumb_unstable_sorter>
    >;

    using adapted_stable_sorter = cppsort::stable_adapter<
        cppsort::self_sort_adapter<dumb_stable_sorter>
    >;

    SECTION( "is_always_stable" )
    {
        STATIC_CHECK( cppsort::is_always_stable<adapted_unstable_sorter>::value );
        STATIC_CHECK( cppsort::is_always_stable<adapted_stable_sorter>::value );
    }

    SECTION( "is_stable" )
    {
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_none&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_stable_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_both&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_unstable_sorter(container_both::iterator,
                                                                 container_both::iterator)>::value );

        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_none&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_stable_sort&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_both&)>::value );
        STATIC_CHECK( cppsort::is_stable<adapted_stable_sorter(container_both::iterator,
                                                               container_both::iterator)>::value );
    }
}

TEST_CASE( "stable_adapter<hybrid_adapter<self_sort_adapter>>",
           "[self_sort_adapter][stable_adapter][hybrid_adapter]" )
{
    using sorter = cppsort::stable_adapter<
        cppsort::hybrid_adapter<
            cppsort::self_sort_adapter<
                cppsort::selection_sorter
            >
        >
    >;

    container_both container;
    auto res = sorter{}(container);
    CHECK( res == kind::stable_sort );
}
