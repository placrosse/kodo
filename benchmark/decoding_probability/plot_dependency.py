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

    if args.json:
        df = pd.read_json(args.jsonfile)
        df['buildername'] = "local"
    else:
        query = {
        "branch" : "master",
        "scheduler": "kodo-nightly-benchmark",
        "utc_date" : {"$gte": ph.yesterday(), "$lt": ph.today()}
        }
        df = ph.get_dataframe("kodo_decoding_probability", query)

    # Calculate the expected number of extra pacekts depending on the rank
    df['dependency'] = df['rank'].apply(sp.mean, axis=0)-1

    # Group by type of code; dense, sparse
    sparse = df[df['testcase'] == "SparseFullRLNC"].groupby(by= ['buildername',
        'symbol_size','symbols'])
    dense = df[df['testcase'] != "SparseFullRLNC"].groupby(by= ['buildername',
        'symbol_size','symbols'])

    def set_comparison_plot():
        plotter.set_title(buildername)
        pl.xlabel("Rank Deficiency")
        pl.ylabel("Extra Packets")
        pl.xticks( symbols-2**sp.arange(sp.log2(symbols))[::-1] ,
            2**sp.arange(sp.log2(symbols),dtype=int)[::-1])
        pl.grid('on')

    plotter.set_type("sparse")
    for (buildername, symbol_size, symbols), group in sparse:

        #Verbose plotting since due to no pandas support for plotting of vectors
        pl.figure()
        for (deps, field,density) in zip(group['dependency'],
            group['benchmark'], group['density']):
            pl.plot(sp.arange(symbols), deps, marker = ph.markers(field),
                label = "(" + field +", " + str(density) + ")")
        set_comparison_plot()
        plotter.write(buildername)

    plotter.set_type("dense")
    for (buildername, symbol_size, symbols), group in dense:

        #Verbose plotting since due to no pandas support for plotting of vectors
        pl.figure()
        for (deps, field,testcase) in zip(group['dependency'],
            group['benchmark'], group['testcase']):
            pl.plot(sp.arange(symbols), deps, marker = ph.markers(field),
                label = "(" + field +", " + testcase + ")")
        set_comparison_plot()
        plotter.write(buildername)

if __name__ == '__main__':
    args = ph.add_arguments(["json", "output-format"])
    plot(args)
