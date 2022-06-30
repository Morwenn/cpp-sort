/*
 * Copyright (c) 2016-2022 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <utility>
#include <vector>
#include <catch2/catch_test_macros.hpp>
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
        iter_swap(vec.data(), vec.data() + 1);
        CHECK( vec.front().swap_flag );
        CHECK( not vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }

    SECTION( "custom iter_swap is found" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_swap::iterator<adl_swap::foo*>{vec.data()},
            adl_iter_swap::iterator<adl_swap::foo*>{vec.data() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }

    SECTION( "custom iter_move is found" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_move::iterator<adl_swap::foo*>{vec.data()},
            adl_iter_move::iterator<adl_swap::foo*>{vec.data() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( not vec.front().iter_swap_flag );
        CHECK( vec.front().iter_move_flag );
    }

    SECTION( "custom iter_swap is found even when iter_move is there" )
    {
        std::vector<adl_swap::foo> vec = { {}, {} };

        using cppsort::utility::iter_swap;
        iter_swap(
            adl_iter_swap_and_move::iterator<adl_swap::foo*>{vec.data()},
            adl_iter_swap_and_move::iterator<adl_swap::foo*>{vec.data() + 1}
        );
        CHECK( not vec.front().swap_flag );
        CHECK( vec.front().iter_swap_flag );
        CHECK( not vec.front().iter_move_flag );
    }
}
