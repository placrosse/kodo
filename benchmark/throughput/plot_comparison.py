#!/usr/bin/env python
"""
Copyright Steinwurf ApS 2011-2013.
Distributed under the "STEINWURF RESEARCH LICENSE 1.0".
See accompanying file LICENSE.rst or
http://www.steinwurf.com/licensing

Plot the difference in throughput between all branches where the benchmarks have
been run within the last 72 hours for all supported Kodo platforms

In order for this script to output some comparison figures you should force the
relevant benchmark run on our buildslaves: http://176.28.49.184:12344/buildslaves
"""

import argparse
import pandas as pd
import scipy as sp
import pylab as pl

import sys
sys.path.insert(0, "../")
import plot_helper as ph

def plot(args):
    plotter = ph.plotter(args)

    query_branches = {
    "type": args.coder,
    "scheduler": "kodo-force-benchmark",
    "utc_date" : {"$gte": args.date - ph.timedelta(1), "$lt": args.date}
    }

    query_master = {
    "type": args.coder,
    "branch" : "master",
    "scheduler": "kodo-nightly-benchmark",
    "utc_date" : {"$gte": args.date - ph.timedelta(1), "$lt": args.date}
    }

    df_master = plotter.get_dataframe(query_master, "kodo_throughput")
    df_branches = plotter.get_dataframe(query_branches, "kodo_throughput")
    df_all = df_master.append(df_branches)

    df_all['mean'] = df_all['throughput'].apply(sp.mean)
    df_all['std'] = df_all['throughput'].apply(sp.std)
    groups = df_all.groupby(['slavename'])

    branches = list(sp.unique(df_all['branch']))
    if len(branches) == 1:
        print("Only recent benchmarks for the master branch in the database, "
              "no plots will be generated.")

    for slavename, group in groups:

        # Group all results from the most recent master build
        master_group = group[sp.array(group['branch'] == "master")]
        group[group['branch'] == "master"]
        if len(master_group) == 0:
            print "Skipping " + slavename + " as no nightly benchmark results \
                exists for the master for this buider yet"
            continue
        master_group = master_group[master_group['buildnumber'] == \
            max(master_group['buildnumber'])]

        # Group all other results by branch
        branches_group = group[group['branch'] != "master"].groupby(by = ['branch'])

        for branch, branch_group in branches_group:
            plotter = ph.plotter(args)
            plotter.set_branch(branch)

            # Calculate the difference compared to master of the latest build
            branch_group = branch_group[branch_group["buildnumber"] \
                == max(branch_group['buildnumber'])]
            branch_group['gain'] = (sp.array(branch_group['mean']) - \
                sp.array(master_group['mean']) ) / sp.array(master_group['mean'])*100

            # Group by type of code; dense, sparse
            dense = branch_group[branch_group['testcase'] != \
                "SparseFullRLNC"].groupby(by= ['symbol_size'])
            sparse = branch_group[branch_group['testcase'] == \
                "SparseFullRLNC"].groupby(by= ['symbol_size'])

            def plot_setup(p):
                pl.ylabel("Throughput gain [\%]")
                pl.xticks(list(sp.unique(group['symbols'])))
                plotter.set_title(slavename)
                plotter.set_markers(p)
                plotter.set_slave_info(slavename)

            for key, g in sparse:
                p = g.pivot_table('gain',  rows='symbols', cols=['benchmark',
                    'density']).plot()
                plot_setup(p)
                plotter.write("spare", slavename)

            for key, g in dense:
                p = g.pivot_table('gain',  rows='symbols',
                    cols=['benchmark','testcase']).plot()
                plot_setup(p)
                plotter.write("dense", slavename)

    return df_all

if __name__ == '__main__':
    args = ph.add_arguments(["coder", "date", "days", "output-format"])
    df = plot(args)
