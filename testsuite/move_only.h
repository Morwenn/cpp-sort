/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2015-2019 Morwenn
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
#ifndef CPPSORT_TESTSUITE_MOVE_ONLY_H_
#define CPPSORT_TESTSUITE_MOVE_ONLY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <stdexcept>
#include <utility>

////////////////////////////////////////////////////////////
// Move-only type for benchmarks
//
// std::sort and std::stable_sort are supposed to be able to
// sort collections of types that are move-only and that are
// not default-constructible. The class template move_only
// wraps such a type and can be fed to algorithms to check
// whether they still compile.
//
// Additionally, move_only detects attempts to read the value
// after a move has been performed and throws an exceptions
// when it happens.
//
// It also checks that no self-move is performed, since the
// standard algorithms can't rely on that to work either.
//

inline namespace i_need_a_namespace_for_proper_adl
{
    template<typename T>
    struct move_only
    {
        // Not default-constructible
        move_only() = delete;

        // Move-only
        move_only(const move_only&) = delete;
        move_only& operator=(const move_only&) = delete;

        // Can be constructed from a T for convenience
        move_only(const T& value):
            can_read(true),
            value(value)
        {}

        // Move operators

        move_only(move_only&& other):
            can_read(true),
            value(std::move(other.value))
        {
            if (not std::exchange(other.can_read, false)) {
                throw std::logic_error("illegal read from a moved-from value");
            }
        }

        auto operator=(move_only&& other)
            -> move_only&
        {
            if (&other == this) {
                throw std::logic_error("illegal self-move was performed");
            }

            if (not std::exchange(other.can_read, false)) {
                throw std::logic_error("illegal read from a moved-from value");
            }
            can_read = true;
            value = std::move(other.value);

            return *this;
        }

        // Whether the value can be read
        bool can_read = false;
        // Actual value
        T value;
    };

    template<typename T>
    auto operator<(const move_only<T>& lhs, const move_only<T>& rhs)
        -> bool
    {
        return lhs.value < rhs.value;
    }

    template<typename T>
    auto swap(move_only<T>& lhs, move_only<T>& rhs)
        -> void
    {
        // This function matters because we want to prevent self-moves
        // but we don't want to prevent self-swaps because it is the
        // responsibility of class authors to make sure that self-swap
        // does the right thing, and not the responsibility of algorithm
        // authors to prevent them from happening

        // Both operands need to be readable
        if (not (lhs.can_read || rhs.can_read)) {
            throw std::logic_error("illegal read from a moved-from value");
        }

        // Swapping the values is enough to preserve the preconditions
        using std::swap;
        swap(lhs.value, rhs.value);
    }
}

#endif // CPPSORT_TESTSUITE_MOVE_ONLY_H_
