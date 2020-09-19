# -*- coding: utf-8 -*-

# Copyright (c) 2020 Morwenn
# SPDX-License-Identifier: MIT

import argparse
import pathlib
import sys

import numpy
from matplotlib import pyplot


def main():
    parser = argparse.ArgumentParser(description="Plot the benchmark results.")
    parser.add_argument('root', help="directory with the resut files to plot")
    parser.add_argument('--alternative-palette', dest='use_alt_palette',
                        action='store_true', default=False,
                        help="Use another color palette")
    args = parser.parse_args()

    root = pathlib.Path(args.root)
    result_files = list(root.glob('*.csv'))
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
    markers = iter(['o', '^', 's', 'p', 'P', '*', 'H', 'X', 'D', 'v'])

    ax = pyplot.gca()

    for result_file in result_files:
        with result_file.open() as fd:
            # Read the first line
            algo_name = fd.readline().strip()
            # Read the rest of the file
            data = numpy.genfromtxt(fd, delimiter=',').transpose()
            pow_of_2, size, avg, stddev = data
        # Add result to graph
        ax.errorbar(
            size,
            avg / 1000.0,
            yerr=stddev / 1000.0, 
            label=algo_name,
            color=next(colors),
            marker=next(markers)
        )

    ax.grid(True)
    ax.set_xlabel('Size')
    ax.set_ylabel('Time [s]')
    ax.set_xscale('log', basex=2)
    ax.set_yscale('log')
    pyplot.xlim(pyplot.xlim()[0] / 2, pyplot.xlim()[1] * 2)

    pyplot.title("Sorting std::vector<int>")
    pyplot.legend(loc='best')

    figure = pyplot.gcf()
    figure.set_size_inches(10, 6)

    pyplot.show()


if __name__ == '__main__':
    main()
