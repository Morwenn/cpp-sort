# -*- coding: utf-8 -*-

# Copyright (c) 2015-2022 Morwenn
# SPDX-License-Identifier: MIT

import sys
from matplotlib import pyplot


if __name__ == '__main__':
    # Name and results of timing functions
    names = []
    values = []

    # Fetch the results
    with open(sys.argv[1]) as fd:
        for line in fd:
            name, *results = line.rstrip().rstrip(',').split(',')
            # Get sorter name
            names.append(name)
            # Plot the results
            results = list(map(int, results))
            xaxis = list(range(1, len(results) + 1))
            val, = pyplot.plot(xaxis, results)
            values.append(val)

    # Add a legend
    pyplot.legend(values, names)
    pyplot.title("Sorting std::array<int>")
    pyplot.xlabel("Number of elements to sort")
    pyplot.ylabel("Cycles per element (lower is better)")
    pyplot.show()
