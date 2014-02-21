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
from matplotlib import pyplot as pl

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
    df = plotter.get_dataframe(query, "kodo_decoding_probability")

    # Calculate the expected number of extra pacekts depending on the rank
    df['dependency'] = df['rank'].apply(sp.mean, axis=0)-1
    df['field'] = df['benchmark'].apply(ph.fields)
    df['algorithm'] = df['testcase'].apply(ph.algorithms)

    # Group by type of code; dense, sparse
    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['slavename',
        'symbol_size','symbols'])
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['slavename',
        'symbol_size','symbols'])

    def set_comparison_plot():
        #pl.xlim(xmin = max(0, pl.xlim()[1] -16 ))
        pl.xticks( symbols-2**sp.arange(sp.log2(symbols))[::-1] ,
            2**sp.arange(sp.log2(symbols),dtype=int)[::-1])
        pl.grid('on')
        plotter.set_slave_info(slavename)
        pl.xlabel("Rank Deficiency")
        pl.ylabel("Extra Packets")

    #Verbose plotting since due to no pandas support for plotting of vectors
    for (slavename, symbol_size, symbols), group in sparse:
        pl.figure()
        for (deps, field,density) in zip(group['dependency'], group['field'],
            group['density']):
            pl.plot(sp.arange(symbols), deps, marker = ph.markers(field),
                label = "(" + field +", " + str(density) + ")")
        set_comparison_plot()
        plotter.write("sparse", slavename)

    for (slavename, symbol_size, symbols), group in dense:
        pl.figure()
        for (deps, field, algorithm) in zip(group['dependency'], group['field'],
            group['algorithm']):
            pl.plot(sp.arange(symbols), deps, marker = ph.markers(field),
                label = "(" + field +", " + algorithm + ")")
        set_comparison_plot()
        plotter.write("dense", slavename)

    return df

if __name__ == '__main__':
    args = ph.add_arguments(["json", "date", "output-format"])
    df = plot(args)
