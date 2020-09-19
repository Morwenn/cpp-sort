/*
 * Copyright (c) 2017 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_INTERNAL_COMPARE_H_
#define CPPSORT_TESTSUITE_INTERNAL_COMPARE_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>

////////////////////////////////////////////////////////////
// Class with internal comparison function
//
// Some classes happen to provide a Java-style compareTo
// function to compare the current instance with another
// instance passed as a parameter to this function. The
// class internal_compare is such a class and is used to
// check that the library's algorithms can handle such
// functions passed as a pointer to member function.
//

template<typename T>
class internal_compare
{
    private:

        // Let it be default-constructed
        T value;

    public:

        internal_compare() = default;

        internal_compare(const T& value):
            value(value)
        {}

        auto operator=(const T& value)
            -> internal_compare&
        {
            this->value = value;
            return *this;
        }

        auto compare_to(const internal_compare& other) const
            -> bool
        {
            return value < other.value;
        }

        friend auto operator<(const internal_compare<T>& lhs, const internal_compare<T>& rhs)
            -> bool
        {
            return lhs.value < rhs.value;
        }

        friend auto operator>(const internal_compare<T>& lhs, const internal_compare<T>& rhs)
            -> bool
        {
            return lhs.value > rhs.value;
        }

        friend auto operator-(const internal_compare<T>& value)
            -> internal_compare<T>
        {
            return internal_compare<T>(-value.value);
        }
};

#endif // CPPSORT_TESTSUITE_INTERNAL_COMPARE_H_
