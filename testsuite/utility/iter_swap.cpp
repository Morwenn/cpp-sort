/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Morwenn
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
#include <utility>
#include <vector>
#include <catch.hpp>
#include <cpp-sort/utility/iter_move.h>

namespace
{
    namespace adl_swap
    {
        struct foo
        {
            bool swap_flag = false;
            bool iter_swap_flag = false;
            bool iter_move_flag = false;
        };

        auto swap(foo& lhs, foo&)
            -> void
        {
            lhs.swap_flag = true;
        }
    }

    namespace adl_iter_move
    {
        template<typename Iterator>
        struct iterator
        {
            Iterator base;

            auto operator*()
                -> decltype(*base)
            {
                return *base;
            }
        };

        template<typename Iterator>
        auto iter_move(iterator<Iterator> it)
            -> decltype(std::move(*it.base))
        {
            it.base->iter_move_flag = true;
            return std::move(*it.base);
        }
    }

    namespace adl_iter_swap
    {
        template<typename Iterator>
        struct iterator
        {
            Iterator base;

            auto operator*()
                -> decltype(*base)
            {
                return *base;
            }
        };

        template<typename Iterator>
        auto iter_swap(iterator<Iterator> it, iterator<Iterator>)
            -> void
        {
            it.base->iter_swap_flag = true;
        }
    }

    namespace adl_iter_swap_and_move
    {
        template<typename Iterator>
        struct iterator
        {
            Iterator base;

            auto operator*()
                -> decltype(*base)
            {
                return *base;
            }
        };

        template<typename Iterator>
        auto iter_move(iterator<Iterator> it)
            -> decltype(std::move(*it.base))
        {
            it.base->iter_move_flag = true;
            return std::move(*it.base);
        }

        template<typename Iterator>
        auto iter_swap(iterator<Iterator> it, iterator<Iterator>)
            -> void
        {
            it.base->iter_swap_flag = true;
        }
    }
}

TEST_CASE( "ADL tests for swap and iter_swap",
           "[utility][iter_move][iter_swap]" )
{
    SECTION( "custom swap is found" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };

        using cppsort::utility::iter_swap;
        iter_swap(vec.begin(), vec.begin() + 1);
        CHECK( vec.front().swap_flag );
        CHECK( not vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }

    SECTION( "custom iter_swap is found" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };
        using it_t = std::vector<adl_swap::foo>::iterator;

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_swap::iterator<it_t>{vec.begin()},
            adl_iter_swap::iterator<it_t>{vec.begin() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }

    SECTION( "custom iter_move is found" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };
        using it_t = std::vector<adl_swap::foo>::iterator;

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_move::iterator<it_t>{vec.begin()},
            adl_iter_move::iterator<it_t>{vec.begin() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( not vec.front().iter_swap_flag );
        CHECK( vec.front().iter_move_flag );
    }

    SECTION( "custom iter_swap is found even when iter_move is there" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };
        using it_t = std::vector<adl_swap::foo>::iterator;

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_swap_and_move::iterator<it_t>{vec.begin()},
            adl_iter_swap_and_move::iterator<it_t>{vec.begin() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }
}
