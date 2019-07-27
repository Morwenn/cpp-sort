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
#include <iterator>
#include <set>
#include <string>
#include <vector>
#include <catch2/catch.hpp>
#include <cpp-sort/comparators/case_insensitive_less.h>
#include <cpp-sort/comparators/natural_less.h>
#include <cpp-sort/comparators/total_less.h>

namespace
{
    // Hacky struct to make sure that it is not directly constructible
    // from a string but is comparable with one
    struct string_wrapper
    {
        std::vector<char> yay;

        string_wrapper(int value)
        {
            auto str = std::to_string(value);
            std::copy(str.begin(), str.end(), std::back_inserter(yay));
        }

        auto begin()
            -> decltype(yay.begin())
        {
            return yay.begin();
        }

        auto begin() const
            -> decltype(yay.begin())
        {
            return yay.begin();
        }

        auto end()
            -> decltype(yay.end())
        {
            return yay.end();
        }

        auto end() const
            -> decltype(yay.end())
        {
            return yay.end();
        }
    };

    auto total_less(const string_wrapper& lhs, const string_wrapper& rhs)
        -> bool
    {
        return lhs.yay < rhs.yay;
    }

    auto total_less(const string_wrapper& lhs, const std::string& rhs)
        -> bool
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    auto total_less(const std::string& lhs, const string_wrapper& rhs)
        -> bool
    {
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    auto case_insensitive_less(const string_wrapper& lhs, const std::string& rhs)
        -> bool
    {
        // Doesn't make sense, but...
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    auto case_insensitive_less(const std::string& lhs, const string_wrapper& rhs)
        -> bool
    {
        // Doesn't make sense, but...
        return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    auto natural_less(const string_wrapper& lhs, const std::string& rhs)
        -> bool
    {
        return cppsort::detail::natural_less(lhs, rhs);
    }

    auto natural_less(const std::string& lhs, const string_wrapper& rhs)
        -> bool
    {
        return cppsort::detail::natural_less(lhs, rhs);
    }
}

TEST_CASE( "Check that comparators work as transparent comparators" )
{
    using namespace std::string_literals;

    SECTION( "total_less" )
    {
        std::set<string_wrapper, cppsort::total_less_t> strings = { 1, 2, 3, 4 };

        CHECK( strings.find("1"s) != strings.end() );
        CHECK( strings.find("8"s) == strings.end() );
    }

    SECTION( "case_insensitive_less" )
    {
        std::set<string_wrapper, cppsort::case_insensitive_less_t> strings = { 1, 2, 3, 4 };

        CHECK( strings.find("1"s) != strings.end() );
        CHECK( strings.find("8"s) == strings.end() );
    }

    SECTION( "natural_less" )
    {
        std::set<string_wrapper, cppsort::natural_less_t> strings = { 1, 2, 3, 4 };

        CHECK( strings.find("1"s) != strings.end() );
        CHECK( strings.find("8"s) == strings.end() );
    }
}
