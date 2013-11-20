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

import sys
sys.path.insert(0, "../")
import processing_shared as ps

from datetime import datetime, timedelta
now = datetime.utcnow()
today = now.date()
today = datetime(today.year, today.month, today.day)
yesterday = today - timedelta(1)

def plot(args):
    query_branches = {
    "type": args.coder,
    "scheduler": "kodo-force-benchmark",
    "utc_date" : {"$gte": now - timedelta(args.days)}
    }

    query_master = {
    "type": args.coder,
    "branch" : "master",
    "scheduler": "kodo-nightly-benchmark",
    "utc_date" : {"$gte": yesterday, "$lt": today}
    }

    db = ps.connect_database()
    cursor_master = db.kodo_throughput.find(query_master)
    cursor_branches = db.kodo_throughput.find(query_branches)

    df_all = pd.DataFrame.from_records(sp.hstack( [list(cursor_master),
        list(cursor_branches)] ))
    df_all['mean'] = df_all['throughput'].apply(sp.mean)
    df_all['std'] = df_all['throughput'].apply(sp.std)
    groups = df_all.groupby(['buildername'])

    from matplotlib import pyplot as pl
    from matplotlib.backends.backend_pdf import PdfPages as pp
    pl.close('all')

    PATH  = ("./figures_database/" + args.coder + "/")

    branches = list(sp.unique(df_all['branch']))
    if len(branches) == 1:
        print("Only recent benchmarks for the master branch in the database, "
              "no plots will be generated.")

    pdf = {}
    for branch in branches:
        if branch != "master":
            ps.mkdir_p(PATH + branch.replace("-","_") + "/sparse")
            ps.mkdir_p(PATH  + branch.replace("-","_") + "/dense")
            pdf[branch] = pp(PATH + branch.replace("-","_") + "/all.pdf")

    for buildername, group in groups:

        # Group all results from the most recent master build
        master_group = group[sp.array(group['branch'] == "master")]
        group[group['branch'] == "master"]
        if len(master_group) == 0:
            print "Skipping " + buildername + " as no nightly benchmark results \
                exists for the master for this buider yet"
            continue
        master_group = master_group[master_group['buildnumber'] == \
            max(master_group['buildnumber'])]

        # Group all other results by branch
        branches_group = group[group['branch'] != "master"].groupby(by = ['branch'])

        for branch, branch_group in branches_group:
            PATH_BRANCH  = PATH + (branch ).replace("-","_")

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

            for key, g in sparse:
                ps.set_sparse_plot()
                p = g.pivot_table('gain',  rows='symbols', cols=['benchmark',
                    'density']).plot()
                ps.set_plot_details(p, buildername)
                pl.ylabel("Throughput gain [\%]")
                pl.xticks(list(sp.unique(group['symbols'])))
                pl.savefig(PATH_BRANCH + "/sparse/" + buildername + "." + args.format)
                pdf[branch].savefig(transparent=True)

            for key, g in dense:
                ps.set_dense_plot()
                p = g.pivot_table('gain',  rows='symbols',
                    cols=['benchmark','testcase']).plot()
                ps.set_plot_details(p, buildername)
                pl.ylabel("Throughput gain [\%]")
                pl.xticks(list(sp.unique(group['symbols'])))
                pl.savefig(PATH_BRANCH + "/dense/" + buildername + "." + args.format)
                pdf[branch].savefig(transparent=True)

    for p in pdf:
        pdf[p].close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '--coder', dest='coder', action='store', choices=['encoder','decoder'],
        default='decoder',
        help='Whether to consider the encoding or decoding performance'
        )
    parser.add_argument(
        '--days', dest='days', type=int, action='store', default=3,
        help='How many days to look back in time when comparing')
    parser.add_argument(
        '--output-format', dest='format', action='store', default='eps',
        help='The format of the generated figures, e.g. eps, pdf')


    args = parser.parse_args()
    plot(args)
