/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2020 Morwenn
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
