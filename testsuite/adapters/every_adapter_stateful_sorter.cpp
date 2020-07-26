/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Morwenn
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
#include <type_traits>
#include <utility>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/adapters.h>
#include <cpp-sort/fixed_sorters.h>
#include <cpp-sort/sorters/quick_merge_sorter.h>
#include <testing-tools/distributions.h>

namespace
{
    template<typename IteratorCategory>
    struct stateful_sorter_impl
    {
        int dummy = 0;

        stateful_sorter_impl() = default;

        constexpr stateful_sorter_impl(int a):
            dummy(a)
        {}

        template<
            typename Iterator,
            typename Compare = std::less<>,
            typename = std::enable_if_t<
                not cppsort::is_projection_iterator_v<Compare, Iterator>
            >
        >
        auto operator()(Iterator first, Iterator last, Compare compare={}) const
            -> int
        {
            cppsort::quick_merge_sort(std::move(first), std::move(last), std::move(compare));
            if (std::is_same<IteratorCategory, std::forward_iterator_tag>::value) {
                return 1;
            } else if (std::is_same<IteratorCategory, std::bidirectional_iterator_tag>::value) {
                return 2;
            } else {
                return 3;
            }
        }

        using iterator_category = IteratorCategory;
        using is_always_stable = std::false_type;
    };

    template<typename IteratorCategory=std::forward_iterator_tag>
    struct stateful_sorter:
        cppsort::sorter_facade<stateful_sorter_impl<IteratorCategory>>
    {
        stateful_sorter() = default;
        stateful_sorter(int a):
            cppsort::sorter_facade<stateful_sorter_impl<IteratorCategory>>(a)
        {}
    };
}

TEST_CASE( "test stateful sorters with every adapter",
           "[adapters][adapter_storage]" )
{
    auto distribution = dist::shuffled{};

    // Collections to sort
    std::vector<short int> collection; collection.reserve(15);
    distribution(std::back_inserter(collection), 15, 0);
    std::list<short int> li;
    distribution(std::back_inserter(li), 15, 0);
    std::forward_list<short int> fli;
    distribution(std::front_inserter(fli), 15, 0);

    SECTION( "container_aware_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::container_aware_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "counting_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::counting_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "hybrid_adapter" )
    {
        stateful_sorter<std::forward_iterator_tag> sorter1(41);
        stateful_sorter<std::bidirectional_iterator_tag> sorter2(42);
        stateful_sorter<std::random_access_iterator_tag> sorter3(43);
        cppsort::hybrid_adapter<
            stateful_sorter<std::forward_iterator_tag>,
            stateful_sorter<std::bidirectional_iterator_tag>,
            stateful_sorter<std::random_access_iterator_tag>
        > sort_it(sorter1, sorter2, sorter3);

        int res1 = sort_it(fli, std::greater<>{});
        CHECK( res1 == 1 );
        CHECK( std::is_sorted(std::begin(fli), std::end(fli), std::greater<>{}) );

        int res2 = sort_it(li, std::greater<>{});
        CHECK( res2 == 2 );
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );

        int res3 = sort_it(collection, std::greater<>{});
        CHECK( res3 == 3 );
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "indirect_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::indirect_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "out_of_place_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::out_of_place_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );

        sort_it(fli, std::greater<>{});
        CHECK( std::is_sorted(std::begin(fli), std::end(fli), std::greater<>{}) );
    }

    SECTION( "schwartz_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::schwartz_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "self_sort_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::self_sort_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "stable_adapter<self_sort_adapter>" )
    {
        stateful_sorter<> sorter(42);
        auto sort_it = cppsort::stable_adapter<
            cppsort::self_sort_adapter<stateful_sorter<>>
        >(cppsort::self_sort_adapter<stateful_sorter<>>(sorter));

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );

        sort_it(li, std::greater<>{});
        CHECK( std::is_sorted(std::begin(li), std::end(li), std::greater<>{}) );
    }

    SECTION( "stable_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::stable_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }

    SECTION( "verge_adapter" )
    {
        stateful_sorter<> sorter(42);
        cppsort::verge_adapter<stateful_sorter<>> sort_it(sorter);

        sort_it(collection, std::greater<>{});
        CHECK( std::is_sorted(std::begin(collection), std::end(collection), std::greater<>{}) );
    }
}
