/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#ifndef CPPSORT_TESTSUITE_WRAPPER_H_
#define CPPSORT_TESTSUITE_WRAPPER_H_

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <iterator>

////////////////////////////////////////////////////////////
// Wrapper around a simple type
//
// This wrapper with a public value is mostly used to test
// pointer to data members used a projections throughout
// the test suite

template<typename T>
struct generic_wrapper
{
    T value;
};

////////////////////////////////////////////////////////////
// Wrapper with an "order"
//
// This wrapper has an additional integral field meant to
// attach more information to it than the value it wraps,
// information which can be used to try to detect issues in
// stable sorting algorithms

template<typename T>
struct generic_stable_wrapper
{
    T value;
    int order;

    friend auto operator<(const generic_stable_wrapper& lhs, const generic_stable_wrapper& rhs)
        -> bool
    {
        if (lhs.value < rhs.value) {
            return true;
        }
        if (rhs.value < lhs.value) {
            return false;
        }
        return lhs.order < rhs.order;
    }
};


#endif // CPPSORT_TESTSUITE_WRAPPER_H_
