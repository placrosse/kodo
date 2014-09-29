#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the number of extra symbols needed to decode
"""

import scipy
from matplotlib import pyplot

import sys

import os
sys.path.insert(0, os.path.join(
    os.path.dirname(os.path.realpath(__file__)), ".."))

import plot_helper


def plot(args):
    plotter = plot_helper.plotter(args)

    query = {
        "branch": "master",
        "scheduler": "kodo (nightly benchmark)",
        "utc_date": {
            "$gte": args.date - plot_helper.timedelta(1),
            "$lt": args.date},
    }
    df = plotter.get_dataframe(query, "kodo_decoding_probability")

    # Calculate the expected number of extra pacekts depending on the rank
    df['dependency'] = df['rank'].apply(scipy.mean, axis=0) - 1
    df['field'] = df['benchmark'].apply(plot_helper.get_field)
    df['algorithm'] = df['testcase'].apply(plot_helper.get_algorithm)

    # Group by type of code; dense, sparse
    dense = df[df['testcase'].isin(plot_helper.codes['dense'])].groupby(
        by=['slavename', 'symbol_size', 'symbols'])
    sparse = df[df['testcase'].isin(plot_helper.codes['sparse'])].groupby(
        by=['slavename', 'symbol_size', 'symbols'])

    def set_comparison_plot():
        #pl.xlim(xmin = max(0, pl.xlim()[1] -16 ))
        pyplot.xticks(
            symbols - 2 ** scipy.arange(scipy.log2(symbols))[::-1],
            2 ** scipy.arange(scipy.log2(symbols), dtype=int)[::-1])
        pyplot.grid('on')
        plotter.set_slave_info(slavename)
        pyplot.xlabel("Rank Deficiency")
        pyplot.ylabel("Extra Packets")

    # Verbose plotting since due to no pandas support for plotting of vectors
    for (slavename, symbol_size, symbols), group in sparse:
        pyplot.figure()
        for (deps, field, density) in zip(group['dependency'], group['field'],
                                          group['density']):
            pyplot.plot(
                scipy.arange(symbols),
                deps,
                marker=plot_helper.get_marker(field),
                label="({}, {})".format(field, str(density)))

        set_comparison_plot()
        plotter.write("sparse", slavename)

    for (slavename, symbol_size, symbols), group in dense:
        pyplot.figure()
        for (deps, field, algorithm) in zip(
                group['dependency'],
                group['field'],
                group['algorithm']):
            pyplot.plot(
                scipy.arange(symbols),
                deps,
                marker=plot_helper.get_marker(field),
                label="({}, {})".format(field, algorithm))
        set_comparison_plot()
        plotter.write("dense", slavename)

    return df

if __name__ == '__main__':
    args = plot_helper.add_arguments(["json", "date", "output-format"])
    df = plot(args)
