/*
 * Copyright (c) 2015-2021 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_MOVE_ONLY_H_
#define CPPSORT_TESTSUITE_MOVE_ONLY_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <stdexcept>
#include <unordered_set>
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
        // Keep track of the constructed instances() of move_only<T>
        static std::unordered_set<move_only*> instances;


        // Not default-constructible
        move_only() = delete;

        // Move-only
        move_only(const move_only&) = delete;
        move_only& operator=(const move_only&) = delete;

        // Can be constructed from a T for convenience
        move_only(const T& value):
            can_read(true),
            value(value)
        {
            instances.insert(this);
        }

        // Move operators

        move_only(move_only&& other):
            can_read(true),
            value(std::move(other.value))
        {
            if (instances.find(&other) == instances.end()) {
                throw std::logic_error("read from a ghost move_only instance");
            }
            if (not std::exchange(other.can_read, false)) {
                throw std::logic_error("illegal read from a moved-from value");
            }
            instances.insert(this);
        }

        ~move_only()
        {
            instances.erase(this);
        }

        auto operator=(move_only&& other)
            -> move_only&
        {
            // Ensure that the current instance and the other instance were
            // correctly constructed and not merely willed into existence
            // from uninitialized memory
            if (instances.find(this) == instances.end()) {
                throw std::logic_error("write to a ghost move_only instance");
            }
            if (instances.find(&other) == instances.end()) {
                throw std::logic_error("read from a ghost move_only instance");
            }

            // Self-move should be ok if the object is already in a moved-from
            // state because it incurs no data loss, but should otherwise be
            // frowned upon
            if (&other == this && can_read) {
                throw std::logic_error("illegal self-move was performed");
            }

            // Assign before overwriting other.can_read
            can_read = other.can_read;
            value = std::move(other.value);

            // If the two objects are not the same and we try to read from an
            // object in a moved-from state, then it's a hard error because
            // data might be lost
            if (not std::exchange(other.can_read, false) && &other != this) {
                throw std::logic_error("illegal read from a moved-from value");
            }

            return *this;
        }

        // Whether the value can be read
        bool can_read = false;
        // Actual value
        T value;
    };

    template<typename T>
    std::unordered_set<move_only<T>*> move_only<T>::instances;

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

        // Ensure that the current instance and the other instance were
        // correctly constructed and not merely willed into existence
        // from uninitialized memory
        if (move_only<T>::instances.find(&lhs) == move_only<T>::instances.end()) {
            throw std::logic_error("write to a ghost move_only instance");
        }
        if (move_only<T>::instances.find(&rhs) == move_only<T>::instances.end()) {
            throw std::logic_error("write a ghost move_only instance");
        }

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
