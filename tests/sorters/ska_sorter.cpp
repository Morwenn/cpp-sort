/*
 * Copyright (c) 2017-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <algorithm>
#include <cstdint>
#include <deque>
#include <iterator>
#include <limits>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include <cpp-sort/sorters/ska_sorter.h>
#include <testing-tools/distributions.h>
#include <testing-tools/random.h>

TEST_CASE( "ska_sorter tests", "[ska_sorter]" )
{
    auto distribution = dist::shuffled{};

    SECTION( "sort with int iterable" )
    {
        std::vector<int> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::ska_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with unsigned int iterators" )
    {
        std::vector<unsigned> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::ska_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

#ifdef __SIZEOF_INT128__
    SECTION( "sort with int128 iterable" )
    {
        std::vector<__int128_t> vec;
        distribution(std::back_inserter(vec), 100'000, -10'000);
        cppsort::ska_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with unsigned int128 iterable" )
    {
        std::vector<__uint128_t> vec;
        distribution(std::back_inserter(vec), 100'000);
        cppsort::ska_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
#endif

    SECTION( "sort with float iterable" )
    {
        std::vector<float> vec;
        distribution.call<float>(std::back_inserter(vec), 100'000);
        cppsort::ska_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with double iterators" )
    {
        std::vector<double> vec;
        distribution.call<double>(std::back_inserter(vec), 100'000);
        cppsort::ska_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }

    SECTION( "sort with std::string" )
    {
        std::vector<std::string> vec;
        for (int i = 0 ; i < 100'000 ; ++i) {
            vec.push_back(std::to_string(i));
        }

        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::ska_sort(vec);
        CHECK( std::is_sorted(vec.begin(), vec.end()) );

        std::shuffle(vec.begin(), vec.end(), hasard::engine());
        cppsort::ska_sort(vec.begin(), vec.end());
        CHECK( std::is_sorted(vec.begin(), vec.end()) );
    }
}

namespace
{
    template<typename T>
    constexpr bool is_ska_sortable = cppsort::detail::is_ska_sortable_v<T>;
}

TEST_CASE( "is_ska_sortable", "[ska_sorter]" )
{
    // This isn't really common, but we will be testing
    // a function that isn't exposed in the interface for
    // the sake of it

    SECTION( "built-in integral types" )
    {
        STATIC_CHECK( is_ska_sortable<bool> );
        STATIC_CHECK( is_ska_sortable<char> );
        STATIC_CHECK( is_ska_sortable<unsigned char> );
        STATIC_CHECK( is_ska_sortable<signed char> );
        STATIC_CHECK( is_ska_sortable<wchar_t> );
        STATIC_CHECK( is_ska_sortable<char16_t> );
        STATIC_CHECK( is_ska_sortable<char32_t> );
        STATIC_CHECK( is_ska_sortable<unsigned short> );
        STATIC_CHECK( is_ska_sortable<signed short> );
        STATIC_CHECK( is_ska_sortable<unsigned int> );
        STATIC_CHECK( is_ska_sortable<signed int> );
        STATIC_CHECK( is_ska_sortable<unsigned long> );
        STATIC_CHECK( is_ska_sortable<signed long> );
        STATIC_CHECK( is_ska_sortable<unsigned long long> );
        STATIC_CHECK( is_ska_sortable<signed long long> );
    }

    SECTION( "built-in floating point types" )
    {
        STATIC_CHECK(( is_ska_sortable<float> ||
                       sizeof(float) != sizeof(std::uint32_t) ||
                       not std::numeric_limits<float>::is_iec559 ));

        STATIC_CHECK(( is_ska_sortable<double> ||
                       sizeof(double) != sizeof(std::uint64_t) ||
                       not std::numeric_limits<double>::is_iec559 ));

        STATIC_CHECK_FALSE( is_ska_sortable<long double> );
    }

    SECTION( "standard collections" )
    {
        // Srings
        STATIC_CHECK( is_ska_sortable<std::string> );
        STATIC_CHECK( is_ska_sortable<std::wstring> );
        STATIC_CHECK( is_ska_sortable<std::basic_string<char16_t>> );

        // Other collections
        STATIC_CHECK( is_ska_sortable<std::vector<int>> );
        STATIC_CHECK( is_ska_sortable<std::vector<std::string>> );
        STATIC_CHECK( is_ska_sortable<std::vector<std::vector<std::string>>> );
        STATIC_CHECK( is_ska_sortable<std::deque<std::vector<long long>>> );
        STATIC_CHECK_FALSE( is_ska_sortable<std::deque<long double>> );
    }

    SECTION( "pairs and tuples" )
    {
        // std::pair
        STATIC_CHECK( is_ska_sortable<std::pair<int, int>> );
        STATIC_CHECK( is_ska_sortable<std::pair<int, std::deque<bool>>> );
        STATIC_CHECK( is_ska_sortable<std::pair<std::vector<std::pair<int, long>>, std::deque<bool>>> );
        STATIC_CHECK_FALSE( is_ska_sortable<std::pair<std::vector<std::pair<int, long double>>, std::deque<bool>>> );

        // std::tuple
        STATIC_CHECK( is_ska_sortable<std::tuple<int>> );
        STATIC_CHECK( is_ska_sortable<std::tuple<long int, int, std::string, std::vector<unsigned long long>>> );
        STATIC_CHECK_FALSE( is_ska_sortable<std::tuple<std::string, std::vector<unsigned long long>, std::deque<long double>>> );
    }
}
