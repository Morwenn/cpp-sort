/*
 * Copyright (c) 2020 Morwenn
 * SPDX-License-Identifier: MIT
 */
#include <cmath>

// Simple statistics functions

template<typename Range>
auto average(const Range& values)
    -> double
{
    double avg = 0.0;
    for (auto value: values) {
        avg += value / double(values.size());
    }
    return avg;
}

template<typename Range>
auto standard_deviation(const Range& values, double avg)
    -> double
{
    double stddev = 0.0;
    for (auto value: values) {
        stddev += (value - avg) * (value - avg) / double(values.size());
    }
    return std::sqrt(stddev);
}
