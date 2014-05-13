#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the throughput for all benchmarked Kodo platforms
"""

import scipy
import pylab

import sys
import os
sys.path.insert(0, os.path.join(
    os.path.dirname(os.path.realpath(__file__)), ".."))

import plot_helper


def plot(args):
    plotter = plot_helper.plotter(args)

    query = {
        "type": args.coder,
        "branch": "master",
        "scheduler": "kodo (nightly benchmark)",
        "utc_date": {
            "$gte": args.date - plot_helper.timedelta(1),
            "$lt": args.date},
    }
    df = plotter.get_dataframe(query, "kodo_throughput")

    df['mean'] = df['throughput'].apply(scipy.mean)
    df['std'] = df['throughput'].apply(scipy.std)
    df['field'] = df['benchmark'].apply(plot_helper.get_field)
    df['algorithm'] = df['testcase'].apply(plot_helper.get_algorithm)

    df['slavename'] = df['slavename'].apply(plot_helper.get_slave_info)

    # Group by type of code; dense, sparse
    dense = df[df['testcase'].isin(plot_helper.codes['dense'])].groupby(
        by=['field', 'algorithm', 'symbol_size'])
    sparse = df[df['testcase'].isin(plot_helper.codes['sparse'])].groupby(
        by=['field', 'density', 'symbol_size'])

    def plot_setup(p):
        pylab.ylabel("Throughput" + " [" + list(group['unit'])[0] + "]")
        pylab.yscale('log')
        pylab.xscale('log', basex=2)
        pylab.xticks(
            list(scipy.unique(group['symbols'])),
            list(scipy.unique(group['symbols'])))
        plotter.set_markers(p)
        plotter.set_legend_columns(3)

    for (field, density, symbols), group in sparse:
        p = group.pivot_table(
            'mean', rows='symbols', cols=['slavename',]).plot()
        plot_setup(p)
        plotter.set_legend_title(
            "Field size: {0}, Density: {1}, Symbols: {2}".format(
            str(field), str(density),str(symbols)))
        plotter.write("sparse", field + "_" + str(density))

    for (field, algorithm, symbols), group in dense:
        p = group.pivot_table(
            'mean', rows='symbols', cols=['slavename']).plot()
        plot_setup(p)
        plotter.set_legend_title(
            "Field size: {0}, Algorithm: {1}, Symbols: {2}".format(
            str(field), str(algorithm),str(symbols)))
        plotter.write("dense", field + "_" + algorithm)

    return df

if __name__ == '__main__':
    args = plot_helper.add_arguments(
        ["json", "coder", "output-format", "date"])
    df = plot(args)
