# -*- coding: utf-8 -*-

# Copyright (c) 2020-2022 Morwenn
# SPDX-License-Identifier: MIT

import argparse
import pathlib
import sys

import numpy
from matplotlib import pyplot


def fetch_results(fresults):
    results = fresults.readline().split(' ')
    results.pop()
    return [float(elem) for elem in results]


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description="Plot the results of the errorbar-plot benchmark.")
    parser.add_argument('root', help="directory with the result files to plot")
    parser.add_argument('--alternative-palette', dest='use_alt_palette',
                        action='store_true', default=False,
                        help="Use another color palette")
    args = parser.parse_args()

    root = pathlib.Path(args.root)
    result_files = list(root.glob('*.csv'))
    result_files.sort()
    if len(result_files) == 0:
        print(f"There are no files to plot in {root}")
        sys.exit(1)

    # Choose the colour palette and markers to use
    if args.use_alt_palette:
        # That one has the advantage of being infinite
        palette = pyplot.cm.rainbow(numpy.linspace(0, 1, len(result_files)))
    else:
        # Colorblind-friendly palette (https://gist.github.com/thriveth/8560036)
        palette = ['#377eb8', '#ff7f00', '#4daf4a',
                   '#f781bf', '#a65628', '#984ea3',
                   '#999999', '#e41a1c', '#dede00']
    colors = iter(palette)

    for result_file in result_files:
        percent_inversions = []
        averages = []
        with result_file.open() as fd:
            # Read the first line
            algo_name = fd.readline().strip()
            # Read the rest of the file
            for line in fd:
                pct, *data = line.strip().split(',')
                data = list(map(int, data))
                percent_inversions.append(pct)
                averages.append(numpy.average(data))

        # Plot the results
        pyplot.plot(list(map(int, percent_inversions)),
                    averages,
                    label=algo_name,
                    color=next(colors))

    # Add a legend
    pyplot.legend()
    pyplot.title("Sorting std::vector<int> with $10^6$ elements")
    pyplot.xlabel("Percentage of inversions")
    pyplot.ylabel("Cycles (lower is better)")
    pyplot.show()
