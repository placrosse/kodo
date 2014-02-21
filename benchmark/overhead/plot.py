#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the number of extra symbols needed to decode
"""

import pandas as pd
import scipy as sp
import pylab as pl

import sys
sys.path.insert(0, "../")
import plot_helper as ph

def plot(args):
    plotter = ph.plotter(args)

    query = {
    "branch" : "master",
    "scheduler": "kodo-nightly-benchmark",
    "utc_date" : {"$gte": args.date - ph.timedelta(1), "$lt": args.date},
    }
    df = plotter.get_dataframe(query, "kodo_overhead")

    df['mean'] = (df['used'].apply(sp.mean) - df['coded'].apply(sp.mean) ) \
        / df['coded'].apply(sp.mean)
    df['field'] = df['benchmark'].apply(ph.fields)
    df['algorithm'] = df['testcase'].apply(ph.algorithms)


    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['slavename',
        'symbol_size'])
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['slavename',
        'symbol_size'])

    def plot_setup(p):
        pl.ylabel("Overhead [\%]")
        pl.yscale('log')
        pl.xscale('log', basex=2)
        pl.xticks(list(sp.unique(group['symbols'])),list(sp.unique(group['symbols'])))
        plotter.set_slave_info(slavename)
        plotter.set_markers(p)

    for (slavename,symbols), group in sparse:
        p = group.pivot_table('mean', rows='symbols',
            cols=['field','density']).plot()
        plot_setup(p)
        plotter.set_legend_title("(Field, Density)")
        plotter.write("sparse", slavename)

    for (slavename,symbols), group in dense:
        p = group.pivot_table('mean',  rows='symbols',
            cols=['field','algorithm']).plot()
        plot_setup(p)
        plotter.set_legend_title("(Field, Algorithm)")
        plotter.write("dense", slavename)

    return df

if __name__ == '__main__':
    args = ph.add_arguments(["json", "date", "output-format"])
    df = plot(args)
