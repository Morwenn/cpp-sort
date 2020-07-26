/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Morwenn
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
