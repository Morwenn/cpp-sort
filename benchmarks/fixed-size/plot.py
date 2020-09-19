# -*- coding: utf-8 -*-

# Copyright (c) 2015-2020 Morwenn
# SPDX-License-Identifier: MIT

import sys
import matplotlib.pyplot as plt


def fetch_results(fresults):
    results = fresults.readline().split(' ')
    results.pop()
    return [int(elem) for elem in results]

    
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
            intresults = [int(elem) for elem in results]
            xaxis = list(range(len(intresults)))
            val, = plt.plot(xaxis, intresults)
            values.append(val)

    # Add a legend
    plt.legend(values, names, loc='upper left')
    plt.xlabel('Number of elements to sort')
    plt.ylabel('Execution time (ms)')
    plt.show()
