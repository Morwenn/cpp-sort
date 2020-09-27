/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_FUNCTIONAL_CHECKS_H_
#define CPPSORT_TESTSUITE_FUNCTIONAL_CHECKS_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <stdexcept>
#include <utility>

////////////////////////////////////////////////////////////
// Move-only type for benchmarks
//
// Comparison and projection functions are std::moved all
// over the place to avoid copies of expensive comparison
// or projection functions. The classes in this header are
// meant to check that no algorithm in the library calls a
// moved-from comparison or projection function.
//

struct move_check_compare
{
    public:
        move_check_compare() = default;
        move_check_compare(const move_check_compare&) = default;
        move_check_compare& operator=(const move_check_compare&) = default;

        move_check_compare(move_check_compare&& other):
            valid(other.valid)
        {
            other.valid = false;
        }

        auto operator=(move_check_compare&& other)
            -> move_check_compare&
        {
            valid = other.valid;
            other.valid = false;
            return *this;
        }

        template<typename T, typename U>
        auto operator()(T&& lhs, U&& rhs)
            -> decltype(std::forward<T>(lhs) < std::forward<U>(rhs))
        {
            if (not valid) {
                throw std::logic_error("illegal use of a moved-from comparison");
            }
            return std::forward<T>(lhs) < std::forward<U>(rhs);
        }

    private:
        bool valid = true;
};

struct move_check_projection
{
    public:
        move_check_projection() = default;
        move_check_projection(const move_check_projection&) = default;
        move_check_projection& operator=(const move_check_projection&) = default;

        move_check_projection(move_check_projection&& other):
            valid(other.valid)
        {
            other.valid = false;
        }

        auto operator=(move_check_projection&& other)
            -> move_check_projection&
        {
            valid = other.valid;
            other.valid = false;
            return *this;
        }

        template<typename T>
        auto operator()(T&& value) const
            -> T&&
        {
            if (not valid) {
                throw std::logic_error("illegal use of a moved-from projection");
            }
            return std::forward<T>(value);
        }

        private:
            bool valid = true;
};

#endif // CPPSORT_TESTSUITE_FUNCTIONAL_CHECKS_H_
