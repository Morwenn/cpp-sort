#  -*- coding: utf-8 -*-

# Copyright (c) 2015-2023 Morwenn
# SPDX-License-Identifier: MIT

# Copyright (c) 2015 Orson Peters <orsonpeters@gmail.com>
#
# This software is provided 'as-is', without any express or implied warranty. In no event will the
# authors be held liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose, including commercial
# applications, and to alter it and redistribute it freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not claim that you wrote the
#    original software. If you use this software in a product, an acknowledgment in the product
#    documentation would be appreciated but is not required.
#
# 2. Altered source versions must be plainly marked as such, and must not be misrepresented as
#    being the original software.
#
# 3. This notice may not be removed or altered from any source distribution.

import argparse
import math
import os
import pathlib
from textwrap import wrap

import numpy
from matplotlib import pyplot


def main():
    parser = argparse.ArgumentParser(description="Plot the results of the patterns benchmark.")
    parser.add_argument('root', help="directory with the result files to plot")
    parser.add_argument('--alternative-palette', dest='use_alt_palette',
                        action='store_true', default=False,
                        help="Use another color palette")
    parser.add_argument('--errorbars', dest='display_errorbars',
                        action='store_true', default=False,
                        help="Display errorbars")
    args = parser.parse_args()

    distribution_names = {
        "shuffled_16_values": "Shuffled (16 values)",
        "shuffled": "Shuffled",
        "all_equal": "All equal",
        "ascending": "Ascending",
        "descending": "Descending",
        "pipe_organ": "Pipe organ",
        "push_front": "Push front",
        "push_middle": "Push middle",
        "ascending_sawtooth": "Ascending sawtooth",
        "descending_sawtooth": "Descending sawtooth",
        "descending_sawtooth": "Descending sawtooth",
        "alternating": "Alternating",
    }

    # Algorithm results will be displayed in this order
    algos = [
        "heap_sort",
        "pdq_sort",
        "quick_sort",
        "ska_sort",
        "spin_sort",
        "std_sort",
    ]

    root = pathlib.Path(args.root)
    for file in root.iterdir():
        data = {}
        for line in file.open():
            size, distribution, algo, *results = [
                string.strip() for string in line.split(',')
            ]
            size = int(size)
            distribution = distribution_names[distribution]
            results = [int(result) for result in results]
            data.setdefault(size, {})
            data[size].setdefault(distribution, {})
            data[size][distribution][algo] = results

        # Choose the colour palette and markers to use
        if args.use_alt_palette:
            # That one has the advantage of being infinite
            palette = pyplot.cm.rainbow(numpy.linspace(0, 1, len(algos)))
        else:
            # Colorblind-friendly palette (https://gist.github.com/thriveth/8560036)
            palette = ['#377eb8', '#ff7f00', '#4daf4a',
                       '#f781bf', '#a65628', '#984ea3',
                       '#999999', '#e41a1c', '#dede00']

        for size in data:
            distributions = (
                "Shuffled",
                "Shuffled (16 values)",
                "All equal",
                "Ascending",
                "Descending",
                "Pipe organ",
                "Push front",
                "Ascending sawtooth",
                "Descending sawtooth",
                "Alternating",
            )

            groupnames = distributions
            groupsize = len(algos)
            groups = [[data[size][distribution][algo] for algo in algos] for distribution in distributions]
            barwidth = 0.6
            spacing = 1
            groupwidth = groupsize * barwidth + spacing

            colors = iter(palette)
            for i, algo in enumerate(algos):
                heights = [numpy.median(data[size][distribution][algo]) for distribution in distributions]
                errors = [numpy.std(data[size][distribution][algo]) for distribution in distributions]
                kwargs = {"xerr": errors} if args.display_errorbars else {}
                pyplot.barh([barwidth * i + groupwidth * n for n in range(len(distributions))],
                            heights, 0.6, color=next(colors), label=algo, **kwargs)

            # Set axes limits and labels.
            groupnames = ['\n'.join(wrap(l, 11)) for l in groupnames]
            pyplot.yticks([barwidth * groupsize/2 + groupwidth*n for n in range(len(groupnames))],
                          groupnames, horizontalalignment='center')
            pyplot.xlabel("Cycles per element (lower is better)")

            # Turn off ticks for y-axis.
            pyplot.tick_params(axis="y",
                               which="both",
                               left="off",
                               right="off",
                               labelleft="on",
                               pad=45)

            ax = pyplot.gca()
            ax.invert_yaxis()
            ax.relim()
            ax.autoscale_view()
            pyplot.ylim(pyplot.ylim()[0] + 1, pyplot.ylim()[1] - 1)

            pyplot.title("Sorting a std::vector<double> with $10^{}$ elements".format(round(math.log(size, 10))))
            pyplot.legend(loc="best")

            figure = pyplot.gcf()
            figure.set_size_inches(10, 6)

            pyplot.show()
            pyplot.clf()


if __name__ == '__main__':
    main()
