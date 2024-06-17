# -*- coding: utf-8 -*-

# Copyright (c) 2020-2024 Morwenn
# SPDX-License-Identifier: MIT

import argparse
import math
import pathlib
import sys

import numpy
from matplotlib import pyplot


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
        averages = []
        disorders = []
        with result_file.open() as fd:
            # Read metadata from the first line
            algo_name, mop_name, size = fd.readline().strip().split(',')

            # Read the rest of the file
            for line in fd:
                disorder, *data = line.strip().split(',')
                data = list(map(int, data))
                averages.append(numpy.median(data))
                disorders.append(int(disorder))

        # Reorder the results
        disorders = numpy.asarray(disorders)
        averages = numpy.asarray(averages)
        sorted_indices = numpy.argsort(disorders)
        disorders = disorders[sorted_indices]
        averages = averages[sorted_indices]

        # Plot the results
        pyplot.plot(list(map(int, disorders)),
                    averages,
                    label=algo_name,
                    color=next(colors))

    # Add global information
    pyplot.xlim(left=0)
    pyplot.ylim(bottom=0)
    pyplot.title("Sorting std::vector<int> with $10^{}$ elements".format(
        round(math.log(int(size), 10)))
    )
    pyplot.xlabel(f"${mop_name}$")
    pyplot.ylabel("Cycles (lower is better)")
    pyplot.legend()

    pyplot.show()
