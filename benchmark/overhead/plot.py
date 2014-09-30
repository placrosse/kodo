#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the number of extra symbols needed to decode
"""

import sys
import os
sys.path.insert(0, os.path.join(
    os.path.dirname(os.path.realpath(__file__)), ".."))

import plot_helper

import scipy
import pylab

def plot(args):
    plotter = plot_helper.plotter(args)

    query = {
        "branch": "master",
        "scheduler": "kodo (nightly benchmark)",
        "utc_date": {
            "$gte": args.date - plot_helper.timedelta(1),
            "$lt": args.date},
    }
    df = plotter.get_dataframe(query, "kodo_overhead")
    total = (df['used'].apply(scipy.mean) - df['coded'].apply(scipy.mean))
    df['mean'] = total / df['coded'].apply(scipy.mean)
    df['field'] = df['benchmark'].apply(plot_helper.get_field)
    df['algorithm'] = df['testcase'].apply(plot_helper.get_algorithm)

    dense = df[df['testcase'].isin(plot_helper.codes['dense'])].groupby(
        by=['slavename', 'symbol_size'])
    sparse = df[df['testcase'].isin(plot_helper.codes['sparse'])].groupby(
        by=['slavename', 'symbol_size'])

    def plot_setup(p):
        pylab.ylabel("Overhead [\%]")
        pylab.yscale('log')
        pylab.xscale('log', basex=2)
        pylab.xticks(
            list(scipy.unique(group['symbols'])),
            list(scipy.unique(group['symbols'])))
        plotter.set_slave_info(slavename)
        plotter.set_markers(p)

    for (slavename, symbols), group in sparse:
        p = group.pivot_table('mean', rows='symbols',
                              cols=['field', 'density']).plot()
        plot_setup(p)
        plotter.set_legend_title("(Field, Density)")
        plotter.write("sparse", slavename)

    for (slavename, symbols), group in dense:
        p = group.pivot_table('mean',  rows='symbols',
                              cols=['field', 'algorithm']).plot()
        plot_setup(p)
        plotter.set_legend_title("(Field, Algorithm)")
        plotter.write("dense", slavename)

    return df

if __name__ == '__main__':
    args = plot_helper.add_arguments(["json", "date", "output-format"])
    df = plot(args)
