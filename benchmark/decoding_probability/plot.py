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
    "utc_date" : {"$gte": ph.yesterday(), "$lt": ph.today()}
    }
    df = plotter.get_dataframe(query, "kodo_decoding_probability")

    df['mean'] = df['used'].apply(sp.mean) -df['symbols']
    df['std'] = df['used'].apply(sp.std)

    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['buildername',
        'symbol_size'])
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['buildername',
        'symbol_size'])

    def plot_setup(p):
        pl.ylabel("Extra symbols" + " [" + list(group['unit'])[0] + "]")
        pl.xticks(list(sp.unique(group['symbols'])))
        plotter.set_title(buildername)
        plotter.set_markers(p)


    plotter.set_type("sparse")
    for (buildername,symbols), group in sparse:
        p = group.pivot_table('mean',  rows='symbols', cols=['benchmark',
            'density']).plot()
        plot_setup(p)
        plotter.write(buildername)

    plotter.set_type("dense")
    for (buildername,symbols), group in dense:
        p = group.pivot_table('mean',  rows='symbols', cols=['benchmark',
            'testcase']).plot()
        plot_setup(p)
        plotter.write(buildername)

if __name__ == '__main__':
    args = ph.add_arguments(["json", "output-format"])
    plot(args)
