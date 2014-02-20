#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the throughput for all benchmarked Kodo platforms
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
    "type": args.coder,
    "branch" : "master",
    "scheduler": "kodo-nightly-benchmark",
    "utc_date" : {"$gte": args.date - ph.timedelta(1), "$lt": args.date},
    }
    df = plotter.get_dataframe(query, "kodo_throughput")

    df['mean'] = df['throughput'].apply(sp.mean)
    df['std'] = df['throughput'].apply(sp.std)
    df['field'] = df['benchmark'].apply(ph.fields)
    df['algorithm'] = df['testcase'].apply(ph.algorithms)

    # Group by type of code; dense, sparse
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by=
        ['slavename', 'symbol_size'])
    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by=
        ['slavename', 'symbol_size'])

    def plot_setup(p):
        pl.ylabel("Throughput" + " [" + list(group['unit'])[0] + "]")
        pl.yscale('log')
        pl.xscale('log', basex=2)
        pl.xticks(list(sp.unique(group['symbols'])),list(sp.unique(group['symbols'])))
        plotter.set_markers(p)
        plotter.set_slave_info(slavename)

    for (slavename, symbols), group in sparse:
        p = group.pivot_table('mean',  rows='symbols', cols=['field',
        'density']).plot()
        plot_setup(p)
        plotter.set_legend_title("(Field, Algorithm)")
        plotter.write("sparse", slavename)

    for (slavename, symbols), group in dense:
        p = group.pivot_table('mean',  rows='symbols', cols=['field',
        'algorithm']).plot()
        plot_setup( p)
        plotter.set_legend_title("(Field, Density)")
        plotter.write("dense", slavename)

    return df

if __name__ == '__main__':

    args = ph.add_arguments(["json", "coder", "output-format", "date"])
    df = plot(args)
