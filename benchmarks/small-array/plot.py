# -*- coding: utf-8 -*-

# Copyright (c) 2015-2021 Morwenn
# SPDX-License-Identifier: MIT

import sys
import matplotlib.pyplot as plt


def fetch_results(fresults):
    results = fresults.readline().split(' ')
    results.pop()
    return [float(elem) for elem in results]

    
if __name__ == '__main__':
    # Name and results of timing functions
    names = []
    values = []
    
    # Fetch the results
    with open(sys.argv[1]) as f:
        for line in f:
            results = line.split(' ')
            # Get sorter name
            names.append(results.pop(0))
            # Remove EOL character
            results.pop()
            # Plot the results
            results = [float(elem) for elem in results]
            xaxis = list(range(len(results)))
            val, = plt.plot(xaxis, results)
            values.append(val)

    # Add a legend
    plt.legend(values, names, loc='upper left')
    plt.title('Sorting std::array<int>')
    plt.xlabel('Number of elements to sort')
    plt.ylabel('Cycles (lower is better)')
    plt.show()
